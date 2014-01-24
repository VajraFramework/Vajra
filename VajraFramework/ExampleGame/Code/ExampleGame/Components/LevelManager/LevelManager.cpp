//
//  LevelManager.cpp
//  Created by Matt Kaufmann on 01/10/14.
//

#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/LevelManager/LevelFileTags.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Prefabs/PrefabLoader.h"

// These includes can probably go away once we load objects from prefabs.
#include "ExampleGame/Components/GameScripts/Ai/AiRoutine.h"
#include "ExampleGame/Components/GameScripts/SampleGameScript.h"
#include "ExampleGame/Components/GameScripts/Units/EnemyUnit.h"
#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/Grid/GridRoom.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/MeshRenderer.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

#include <fstream>

ComponentIdType LevelManager::componentTypeId = COMPONENT_TYPE_ID_LEVEL_MANAGER;

LevelManager::LevelManager() : Component() {
	this->init();
}

LevelManager::LevelManager(Object* object_) : Component(object_) {
	this->init();
}

LevelManager::~LevelManager() {
	this->destroy();
}

void LevelManager::HandleMessage(MessageChunk messageChunk) {
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_FRAME_EVENT:
			this->update();
			break;
	}
}

void LevelManager::LoadLevelFromFile(std::string levelFilename) {
	FRAMEWORK->GetLogger()->dbglog("\nLoading level from level file: %s", levelFilename.c_str());

	XmlParser* parser = new XmlParser();
	parser->ParseXmlFile(levelFilename);

#ifdef DEBUG
	parser->Print();
#endif // DEBUG

	XmlTree* xmlTree = parser->GetXmlTree();
	ASSERT(xmlTree != nullptr, "Got valid xmlTree from parser for level file %s", levelFilename.c_str());
	XmlNode* levelNode = xmlTree->GetRootNode();
	ASSERT(levelNode != nullptr && levelNode->GetName() == LEVEL_TAG, "Got valid level node from xml tree for level file %s", levelFilename.c_str());

	XmlNode* gridNode = levelNode->GetFirstChildByNodeName(GRID_TAG);
	SINGLETONS->GetGridManager()->loadGridDataFromXml(gridNode);

	XmlNode* staticNode = levelNode->GetFirstChildByNodeName(STATIC_TAG);
	this->loadStaticDataFromXml(staticNode);

	XmlNode* unitBaseNode = levelNode->GetFirstChildByNodeName(UNITS_TAG);
	this->loadUnitDataFromXml(unitBaseNode);

	XmlNode* otherDataNode = levelNode->GetFirstChildByNodeName(OTHER_TAG);
	this->loadOtherDataFromXml(otherDataNode);

	XmlNode* cameraNode = levelNode->GetFirstChildByNodeName(CAMERA_TAG);
	this->loadCameraDataFromXml(cameraNode);

	delete parser;
/*	std::ifstream ifs;
	ifs.open(levelFilename, std::ios_base::in);

	ASSERT(!ifs.fail(), "Loading level data from file %s", levelFilename.c_str());

	std::string tag;
	ifs >> tag;
	ASSERT(tag == LEVEL_NAME_TAG, "Reading level name from file %s", levelFilename.c_str());
	ifs >> this->currentLevelName;

	SINGLETONS->GetGridManager()->loadGridDataFromStream(ifs);
	this->loadStaticDataFromStream(ifs);
	this->loadUnitDataFromStream(ifs);
	this->loadCameraDataFromStream(ifs);

	ifs.close();
*/
	this->isPaused = false;
}

void LevelManager::init() {
	//this->shadyCam = nullptr;
	this->isPaused = true;
	this->currentLevelName = "";

	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void LevelManager::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void LevelManager::update() {
	if (!this->isPaused) {

	}
}

void LevelManager::loadStaticDataFromXml(XmlNode* staticNode) {
	XmlNode* staticObjNode = staticNode->GetFirstChildByNodeName(STATIC_OBJECT_TAG);
	while (staticObjNode != nullptr) {
		std::string prefab = staticObjNode->GetAttributeValueS(PREFAB_ATTRIBUTE);
		int westBound      = staticObjNode->GetAttributeValueI(X_ATTRIBUTE);
		int southBound     = staticObjNode->GetAttributeValueI(Z_ATTRIBUTE);
		int objWidth       = staticObjNode->GetAttributeValueI(WIDTH_ATTRIBUTE);
		int objHeight      = staticObjNode->GetAttributeValueI(HEIGHT_ATTRIBUTE);
		float rotation     = staticObjNode->GetAttributeValueF(ROTATION_ATTRIBUTE);

		GameObject* staticObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + prefab + PREFAB_EXTENSION, ENGINE->GetSceneGraph3D());

		// Position and orient the object.
		staticObj->GetTransform()->SetPosition(westBound + objWidth / 2.0f, 0.0f, -(southBound + objHeight / 2.0f));
		staticObj->GetTransform()->SetOrientation(rotation, YAXIS);

		// Add the object to the grid
		SINGLETONS->GetGridManager()->placeStaticObjectOnGrid(staticObj->GetId(), westBound, southBound, objWidth, objHeight);

		staticObjNode = staticObjNode->GetNextSiblingByNodeName(STATIC_OBJECT_TAG);
	}
}

void LevelManager::loadUnitDataFromXml(XmlNode* unitBaseNode) {
	// Start with player units
	XmlNode* unitNode = unitBaseNode->GetFirstChildByNodeName("");
	while (unitNode != nullptr) {
		std::string unitPrefab = unitNode->GetAttributeValueS(PREFAB_ATTRIBUTE);
		GameObject* unitObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + unitPrefab + PREFAB_EXTENSION, ENGINE->GetSceneGraph3D());
		//unitObj->AddComponent<SampleGameScript>();
		unitObj->AddComponent<GridNavigator>();

		// Read the initial position of the unit
		int gX = unitNode->GetAttributeValueI(X_ATTRIBUTE);
		int gZ = unitNode->GetAttributeValueI(Z_ATTRIBUTE);
		float rotation = unitNode->GetAttributeValueF(ROTATION_ATTRIBUTE);

		// Position and orient the unit.
		unitObj->GetTransform()->SetPosition(gX, 0.0f, -gZ);
		unitObj->GetTransform()->SetOrientation(rotation, YAXIS);

		// Add the unit to the grid
		SINGLETONS->GetGridManager()->placeUnitOnGrid(unitObj->GetId(), gX, gZ);

		// Check unit type
		if (unitNode->GetName() == PLAYER_UNIT_TAG) {
			unitObj->AddComponent<PlayerUnit>();
		}
		else if (unitNode->GetName() == ENEMY_UNIT_TAG) {
			unitObj->AddComponent<EnemyUnit>();
			AiRoutine* aiRoutine = unitObj->AddComponent<AiRoutine>();

			// Load the AI routine.
			XmlNode* aiCommandNode = unitNode->GetFirstChildByNodeName(AI_COMMAND_TAG);
			while (aiCommandNode != nullptr) {
				std::string command = aiCommandNode->GetValue();
				aiRoutine->taskStrings.push_back(command);

				aiCommandNode = aiCommandNode->GetNextSiblingByNodeName(AI_COMMAND_TAG);
			}
		}

		unitNode = unitNode->GetNextSiblingByNodeName("");
	}
}

void LevelManager::loadOtherDataFromXml(XmlNode* otherDataNode) {
	XmlNode* zoneNode = otherDataNode->GetFirstChildByNodeName(ZONE_TAG);
	while (zoneNode != nullptr) {
		std::string prefab = zoneNode->GetAttributeValueS(PREFAB_ATTRIBUTE);
		int westBound      = zoneNode->GetAttributeValueI(X_ATTRIBUTE);
		int southBound     = zoneNode->GetAttributeValueI(Z_ATTRIBUTE);
		int zoneWidth      = zoneNode->GetAttributeValueI(WIDTH_ATTRIBUTE);
		int zoneHeight     = zoneNode->GetAttributeValueI(HEIGHT_ATTRIBUTE);

		GameObject* zoneObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + prefab + PREFAB_EXTENSION, ENGINE->GetSceneGraph3D());

		GridZone* zoneComp = zoneObj->GetComponent<GridZone>();
		ASSERT(zoneComp != nullptr, "Object within %s tag has GridZone component", ZONE_TAG);
		zoneComp->SetZoneBounds(westBound, southBound, westBound + zoneWidth, southBound + zoneHeight);

		SINGLETONS->GetGridManager()->AddGridZone(zoneObj->GetId());

		zoneNode = zoneNode->GetNextSiblingByNodeName(ZONE_TAG);
	}

	// TODO [Implement] Other things such as triggerables
}

void LevelManager::loadCameraDataFromXml(XmlNode* /*cameraNode*/) {
	// The level data file will specify which unit the camera should focus on by default

	// Create the ShadyCamera; this should possibly be a prefab as well.
	GameObject* camera = new GameObject(ENGINE->GetSceneGraph3D());
	ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(camera->GetId());
	ShadyCamera* cameraComponent = camera->AddComponent<ShadyCamera>();
	ENGINE->GetSceneGraph3D()->SetMainCameraId(camera->GetId());
	cameraComponent->SetGridManager(SINGLETONS->GetGridManager());

	// TODO [Hack] Figure out a better way to do this.
	int gX = SINGLETONS->GetGridManager()->gridRooms[0]->westBound;
	int gZ = SINGLETONS->GetGridManager()->gridRooms[0]->southBound;
	cameraComponent->MoveToRoom(gX, gZ);
}

void LevelManager::endLevel(bool /*success*/) {
	this->isPaused = true;
	//LevelEnd.EndLevel(success);
}
