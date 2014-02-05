//
//  LevelLoader.cpp
//  Created by Matt Kaufmann on 01/24/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Ai/AiPerception.h"
#include "ExampleGame/Components/GameScripts/Ai/AiRoutine.h"
#include "ExampleGame/Components/GameScripts/Units/EnemyUnit.h"
#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/LevelManager/LevelLoader.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/Components/LevelManager/LevelFileTags.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Prefabs/PrefabLoader.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

UnitType StringToUnitType(std::string str) {
	if (str == "Assassin") {
		return UNIT_TYPE_ASSASSIN;
	}
	if (str == "Thief") {
		return UNIT_TYPE_THIEF;
	}
	return UNIT_TYPE_UNKNOWN;
}

void LevelLoader::LoadLevelFromFile(std::string levelFilename) {
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
	ASSERT(gridNode != nullptr, "Level definition contains <%s> node", GRID_TAG);
	SINGLETONS->GetGridManager()->loadGridDataFromXml(gridNode);

	XmlNode* staticNode = levelNode->GetFirstChildByNodeName(STATIC_TAG);
	// The level doesn't need to have a STATIC_TAG node
	if (staticNode != nullptr) {
		loadStaticDataFromXml(staticNode);
	}

	XmlNode* unitBaseNode = levelNode->GetFirstChildByNodeName(UNITS_TAG);
	ASSERT(unitBaseNode != nullptr, "Level definition contains <%s> node", UNITS_TAG);
	loadUnitDataFromXml(unitBaseNode);

	XmlNode* otherDataNode = levelNode->GetFirstChildByNodeName(OTHER_TAG);
	// The level doesn't need to have an OTHER_TAG node
	if (otherDataNode != nullptr) {
		loadOtherDataFromXml(otherDataNode);
	}

	XmlNode* cameraNode = levelNode->GetFirstChildByNodeName(CAMERA_TAG);
	ASSERT(cameraNode != nullptr, "Level definition contains <%s> node", CAMERA_TAG);
	loadCameraDataFromXml(cameraNode);

	delete parser;
}

void LevelLoader::loadStaticDataFromXml(XmlNode* staticNode) {
	XmlNode* staticObjNode = staticNode->GetFirstChildByNodeName(STATIC_OBJECT_TAG);
	while (staticObjNode != nullptr) {
		std::string prefab = staticObjNode->GetAttributeValueS(PREFAB_ATTRIBUTE);
		int westBound      = staticObjNode->GetAttributeValueI(X_ATTRIBUTE);
		int southBound     = staticObjNode->GetAttributeValueI(Z_ATTRIBUTE);
		int objWidth       = staticObjNode->GetAttributeValueI(WIDTH_ATTRIBUTE);
		int objHeight      = staticObjNode->GetAttributeValueI(HEIGHT_ATTRIBUTE);
		float rotation     = staticObjNode->GetAttributeValueF(ROTATION_ATTRIBUTE) inRadians;

		GameObject* staticObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + prefab + PREFAB_EXTENSION, ENGINE->GetSceneGraph3D());

		// Add the object to the grid
		SINGLETONS->GetGridManager()->placeStaticObjectOnGrid(staticObj->GetId(), westBound, southBound, objWidth, objHeight);

		// Orient the object.
		staticObj->GetTransform()->SetOrientation(rotation, YAXIS);

		staticObjNode = staticObjNode->GetNextSiblingByNodeName(STATIC_OBJECT_TAG);
	}
}

void LevelLoader::loadUnitDataFromXml(XmlNode* unitBaseNode) {
	// Start with player units
	XmlNode* unitNode = unitBaseNode->GetFirstChildByNodeName("");
	while (unitNode != nullptr) {
		std::string unitPrefab = unitNode->GetAttributeValueS(PREFAB_ATTRIBUTE);
		GameObject* unitObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + unitPrefab + PREFAB_EXTENSION, ENGINE->GetSceneGraph3D());

		// Units require a GridNavigator component
		GridNavigator* gNav = unitObj->GetComponent<GridNavigator>();
		ASSERT(gNav != nullptr, "Unit with id %d has GridNavigator component", unitObj->GetId());

		// Read the initial position of the unit
		int gX = unitNode->GetAttributeValueI(X_ATTRIBUTE);
		int gZ = unitNode->GetAttributeValueI(Z_ATTRIBUTE);
		float rotation = unitNode->GetAttributeValueF(ROTATION_ATTRIBUTE) inRadians;

		// Add the unit to the grid
		SINGLETONS->GetGridManager()->placeUnitOnGrid(unitObj->GetId(), gX, gZ);

		// Orient the unit.
		unitObj->GetTransform()->SetOrientation(rotation, YAXIS);

		// Check unit type
		if (unitNode->GetName() == ENEMY_UNIT_TAG) {
			EnemyUnit* enemyUnit = unitObj->GetComponent<EnemyUnit>();
			ASSERT(enemyUnit != nullptr, "Enemy unit with id %d has EnemyUnit component", unitObj->GetId());
			enemyUnit->Activate();

			AiPerception* aiPerception = unitObj->GetComponent<AiPerception>();
			ASSERT(aiPerception != nullptr, "Enemy unit with id %d has AiPerception component", unitObj->GetId());
			aiPerception->Activate();

			AiRoutine* aiRoutine = unitObj->GetComponent<AiRoutine>();
			ASSERT(aiRoutine != nullptr, "Enemy unit with id %d has AiRoutine component", unitObj->GetId());

			// Load the AI routine.
			XmlNode* aiCommandNode = unitNode->GetFirstChildByNodeName(AI_COMMAND_TAG);
			std::vector<std::string> commands;
			while (aiCommandNode != nullptr) {
				std::string command = aiCommandNode->GetValue();
				commands.push_back(command);
				aiCommandNode = aiCommandNode->GetNextSiblingByNodeName(AI_COMMAND_TAG);
			}
			aiRoutine->SetBehavior(commands);
		}

		unitNode = unitNode->GetNextSiblingByNodeName("");
	}
}

void LevelLoader::loadOtherDataFromXml(XmlNode* otherDataNode) {
	XmlNode* zoneNode = otherDataNode->GetFirstChildByNodeName(ZONE_TAG);
	while (zoneNode != nullptr) {
		std::string prefab = zoneNode->GetAttributeValueS(PREFAB_ATTRIBUTE);
		int westBound      = zoneNode->GetAttributeValueI(X_ATTRIBUTE);
		int southBound     = zoneNode->GetAttributeValueI(Z_ATTRIBUTE);
		int zoneWidth      = zoneNode->GetAttributeValueI(WIDTH_ATTRIBUTE);
		int zoneHeight     = zoneNode->GetAttributeValueI(HEIGHT_ATTRIBUTE);
		int eastBound      = westBound + zoneWidth - 1;    // This is a cell coordinate, not an absolute position
		int northBound     = southBound + zoneHeight - 1;  // Likewise here

		GameObject* zoneObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + prefab + PREFAB_EXTENSION, ENGINE->GetSceneGraph3D());

		GridZone* zoneComp = zoneObj->GetComponent<GridZone>();
		ASSERT(zoneComp != nullptr, "Object within %s tag has GridZone component", ZONE_TAG);

		zoneComp->SetZoneBounds(westBound, southBound, eastBound, northBound);

		SINGLETONS->GetGridManager()->AddGridZone(zoneObj->GetId());

		// Check for overloaded components
		XmlNode* compNode = zoneNode->GetFirstChildByNodeName(COMPONENT_TAG);
		while (compNode != nullptr) {
			PrefabLoader::LoadComponentFromComponentNodeInPrefab(zoneObj, compNode);
			compNode = compNode->GetNextSiblingByNodeName(COMPONENT_TAG);
		}

		zoneNode = zoneNode->GetNextSiblingByNodeName(ZONE_TAG);
	}

	// TODO [Implement] Other things such as triggerables
}

void LevelLoader::loadCameraDataFromXml(XmlNode* cameraNode) {
	// The level data file will specify which unit the camera should focus on by default
	std::string unitNameStr = cameraNode->GetAttributeValueS("focus");

	// Create the ShadyCamera; this should possibly be a prefab as well.
	GameObject* camera = new GameObject(ENGINE->GetSceneGraph3D());
	ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(camera->GetId());
	ShadyCamera* cameraComponent = camera->AddComponent<ShadyCamera>();
	ENGINE->GetSceneGraph3D()->SetMainCameraId(camera->GetId());
	cameraComponent->SetGridManager(SINGLETONS->GetGridManager());

	// Find the unit that the camera should focus on
	UnitType uType = StringToUnitType(unitNameStr);
	ObjectIdType id = SINGLETONS->GetGridManager()->GetPlayerUnitIdOfType(uType);
	ASSERT(id != OBJECT_ID_INVALID, "Player unit of type %d exists in level", uType);
	SINGLETONS->GetGridManager()->selectedUnitId = id;
	PlayerUnit* pU = ENGINE->GetSceneGraph3D()->GetGameObjectById(id)->GetComponent<PlayerUnit>();
	// TODO [Implement]: call this when the level starts so the player sees the selection animation
	pU->onSelectedTouch();

	// Set the camera's position
	GameObject* target = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
	GridNavigator* gNav = target->GetComponent<GridNavigator>();
	ASSERT(gNav != nullptr, "Object with id %d has GridNavigator component", id);

	GridCell* cell = gNav->GetCurrentCell();
	ASSERT(cell != nullptr, "Object with id %d is on grid", id);

	cameraComponent->MoveToRoom(cell->x, cell->z);
}
