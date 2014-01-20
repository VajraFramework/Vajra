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
/*
void LevelManager::loadStaticDataFromStream(std::istream& ifs) {
	std::string tag;

	// Static Geometry
	int nStaticObjs;
	ifs >> tag;
	ASSERT(tag == NUM_STATIC_TAG, "Loading static objects for level %s", SINGLETONS->GetLevelManager()->GetCurrentLevelName().c_str());
	ifs >> nStaticObjs;

	for (int i = 0; i < nStaticObjs; ++i) {
		std::string prefab;
		int objWestBound, objSouthBound, objWidth, objHeight; // Cell bounds
		float rotation;
		ifs >> prefab >> objWestBound >> objSouthBound >> objWidth >> objHeight >> rotation;

		// Temporary code; Eventually we should be doing this by reading from a prefab.
		GameObject* staticObj = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(staticObj->GetId());

		// Add the object to the grid
		SINGLETONS->GetGridManager()->placeStaticObjectOnGrid(staticObj->GetId(), objWestBound, objSouthBound, objWidth, objHeight);
		staticObj->GetTransform()->SetOrientation(rotation, YAXIS);
	}
}

void LevelManager::loadUnitDataFromStream(std::istream& ifs) {
	std::string tag;

	// Player Units
	int nPlayerUnits;
	ifs >> tag;
	ASSERT(tag == NUM_PLAYERS_TAG, "Loading player units for level %s", SINGLETONS->GetLevelManager()->GetCurrentLevelName().c_str());
	ifs >> nPlayerUnits;

	for (int i = 0; i < nPlayerUnits; ++i) {
		std::string unitType; // Unit type
		int gX, gZ;           // Starting position
		float rotation;       // Orientation
		ifs >> unitType >> gX >> gZ >> rotation;

		// Temporary code; Eventually we should be doing this by reading from a prefab.
		GameObject* unitObj = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(unitObj->GetId());
		MeshRenderer* unitRenderer = unitObj->AddComponent<MeshRenderer>();
		unitRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "Suzanne.model");
		unitObj->AddComponent<SampleGameScript>();
		unitObj->AddComponent<GridNavigator>();
		unitObj->AddComponent<PlayerUnit>();

		// Add the unit to the grid
		SINGLETONS->GetGridManager()->placeUnitOnGrid(unitObj->GetId(), gX, gZ);
		unitObj->GetTransform()->SetOrientation(rotation, YAXIS);
	}

	// Enemy Units
	int nEnemyUnits;
	ifs >> tag;
	ASSERT(tag == NUM_ENEMIES_TAG, "Loading enemy units for level %s", SINGLETONS->GetLevelManager()->GetCurrentLevelName().c_str());
	ifs >> nEnemyUnits;

	for (int i = 0; i < nEnemyUnits; ++i) {
		std::string unitType; // Unit type
		int gX, gZ;           // Starting position
		float rotation;       // Orientation
		int nCommands;        // Number of AI commands
		ifs >> unitType >> gX >> gZ >> rotation >> nCommands;

		// Temporary code; Eventually we should be doing this by reading from a prefab.
		GameObject* enemyObj = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(enemyObj->GetId());
		MeshRenderer* enemyRenderer = enemyObj->AddComponent<MeshRenderer>();
		enemyRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "Suzanne.model");
		enemyObj->AddComponent<SampleGameScript>();
		enemyObj->AddComponent<GridNavigator>();
		enemyObj->AddComponent<EnemyUnit>();
		AiRoutine* aiRoutine = enemyObj->AddComponent<AiRoutine>();

		// Add the unit to the grid
		SINGLETONS->GetGridManager()->placeUnitOnGrid(enemyObj->GetId(), gX, gZ);
		enemyObj->GetTransform()->SetOrientation(rotation, YAXIS);

		for (int j = 0; j < nCommands; ++j) {
			std::string command;
			ifs >> command;
			aiRoutine->taskStrings.push_back(command);
		}
		aiRoutine->parseTaskStrings();
	}
}

void LevelManager::loadCameraDataFromStream(std::istream& ifs) {
	// The level data file will specify which unit the camera should focus on by default
	std::string tag;
	std::string startingUnit;
	ifs >> tag;
	ASSERT(tag == UNIT_START_TAG, "Loading starting unit for level %s", SINGLETONS->GetLevelManager()->GetCurrentLevelName().c_str());
	ifs >> startingUnit;

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
*/
void LevelManager::loadStaticDataFromXml(XmlNode* staticNode) {
	XmlNode* staticObjNode = staticNode->GetFirstChildByNodeName(STATIC_OBJECT_TAG);
	while (staticObjNode != nullptr) {
		std::string prefab = staticObjNode->GetAttributeValueS(PREFAB_ATTRIBUTE);
		int westBound      = staticObjNode->GetAttributeValueI(X_ATTRIBUTE);
		int southBound     = staticObjNode->GetAttributeValueI(Z_ATTRIBUTE);
		int objWidth       = staticObjNode->GetAttributeValueI(WIDTH_ATTRIBUTE);
		int objHeight      = staticObjNode->GetAttributeValueI(HEIGHT_ATTRIBUTE);
		float rotation     = staticObjNode->GetAttributeValueF(ROTATION_ATTRIBUTE);

		GameObject* staticObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + prefab, ENGINE->GetSceneGraph3D());

		// Add the object to the grid
		SINGLETONS->GetGridManager()->placeStaticObjectOnGrid(staticObj->GetId(), westBound, southBound, objWidth, objHeight);
		staticObj->GetTransform()->SetOrientation(rotation, YAXIS);

		staticObjNode = staticObjNode->GetNextSiblingByNodeName(STATIC_OBJECT_TAG);
	}
}

void LevelManager::loadUnitDataFromXml(XmlNode* unitBaseNode) {
	// Start with player units
	XmlNode* unitNode = unitBaseNode->GetFirstChildByNodeName("");
	while (unitNode != nullptr) {
		std::string unitPrefab = unitNode->GetAttributeValueS(PREFAB_ATTRIBUTE);
		GameObject* unitObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + unitPrefab, ENGINE->GetSceneGraph3D());
		unitObj->AddComponent<SampleGameScript>();
		unitObj->AddComponent<GridNavigator>();

		// Read the initial position of the unit
		int gX = unitNode->GetAttributeValueI(X_ATTRIBUTE);
		int gZ = unitNode->GetAttributeValueI(Z_ATTRIBUTE);
		float rotation = unitNode->GetAttributeValueF(ROTATION_ATTRIBUTE);

		// Add the unit to the grid
		SINGLETONS->GetGridManager()->placeUnitOnGrid(unitObj->GetId(), gX, gZ);
		unitObj->GetTransform()->SetOrientation(rotation, YAXIS);

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
