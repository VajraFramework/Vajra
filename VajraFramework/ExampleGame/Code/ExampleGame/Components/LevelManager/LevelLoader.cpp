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
#include "ExampleGame/Components/LevelManager/LevelFileTags.h"
#include "ExampleGame/Components/LevelManager/LevelLoader.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/Components/Switches/BaseSwitch.h"
#include "ExampleGame/Components/Switches/MultiplexSwitch.h"
#include "ExampleGame/Components/Triggers/Triggerable.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/Prefabs/PrefabLoader.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/ParserTags.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

std::map<int, ObjectIdType> LevelLoader::idsFromXml;

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
	parser->ParseXmlFile(FRAMEWORK->GetFileSystemUtils()->GetDeviceBaseResourcesPath() + "levels/" + levelFilename);

#ifdef DEBUG
	parser->Print();
#endif // DEBUG

	XmlTree* xmlTree = parser->GetXmlTree();
	ASSERT(xmlTree != nullptr, "Got valid xmlTree from parser for level file %s", levelFilename.c_str());
	XmlNode* levelNode = xmlTree->GetRootNode();
	ASSERT(levelNode != nullptr && levelNode->GetName() == LEVEL_TAG, "Got valid level node from xml tree for level file %s", levelFilename.c_str());
	
	XmlNode* gridNode = levelNode->GetFirstChildByNodeName(GRID_TAG);
	VERIFY(gridNode != nullptr, "Level definition contains <%s> node", GRID_TAG);
	SINGLETONS->GetGridManager()->loadGridDataFromXml(gridNode);

	XmlNode* staticNode = levelNode->GetFirstChildByNodeName(STATIC_TAG);
	// The level doesn't need to have a STATIC_TAG node
	if (staticNode != nullptr) {
		loadStaticDataFromXml(staticNode);
	}

	XmlNode* unitBaseNode = levelNode->GetFirstChildByNodeName(UNITS_TAG);
	VERIFY(unitBaseNode != nullptr, "Level definition contains <%s> node", UNITS_TAG);
	loadUnitDataFromXml(unitBaseNode);

	XmlNode* otherDataNode = levelNode->GetFirstChildByNodeName(OTHER_TAG);
	// The level doesn't need to have an OTHER_TAG node
	if (otherDataNode != nullptr) {
		loadOtherDataFromXml(otherDataNode);
	}

	XmlNode* linkBaseNode = levelNode->GetFirstChildByNodeName(LINKS_TAG);
	// The level doesn't need to have a LINKS_TAG node
	if (linkBaseNode != nullptr) {
		loadLinkDataFromXml(linkBaseNode);
	}

	XmlNode* cameraNode = levelNode->GetFirstChildByNodeName(CAMERA_TAG);
	VERIFY(cameraNode != nullptr, "Level definition contains <%s> node", CAMERA_TAG);
	loadCameraDataFromXml(cameraNode);

	delete parser;

	idsFromXml.clear();
	
	GameObject* dlight = new GameObject(ENGINE->GetSceneGraph3D());
	ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(dlight->GetId());
	DirectionalLight* dlightComponent = dlight->AddComponent<DirectionalLight>();
	dlight->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
	dlight->GetTransform()->LookAt(0.5f, 10.0f, 0.5f);
	ENGINE->GetSceneGraph3D()->SetMainDirectionalLightId(dlight->GetId());
	//
	dlightComponent->SetAmbientColor(0.125f, 0.125f, 0.15f, 1.0f);
	dlightComponent->SetDiffuseColor(0.6f, 0.45f, 0.45f, 1.0f);
			
}

LevelType LevelLoader::stringToLevelType(std::string type) {
	if(type == "Infiltration") {
		return LevelType::Infiltration;
	}
	else if(type == "Theft") {
		return LevelType::Theft;
	}
	else if(type == "Assassination") {
		return LevelType::Assassination;
	}
	ASSERT(0, "%s is a valid level type", type.c_str());
	return LevelType::NO_TYPE;
}
void LevelLoader::LoadLevelData(std::vector<LevelData>* levelData) {
	// Load the tutorials
	std::vector<std::string> levelsWithTutorials;
	LevelLoader::LoadTutorialLevelNames(&levelsWithTutorials);

	FRAMEWORK->GetLogger()->dbglog("\nLoading levelData from file: %s", levelListXmlPath);

	XmlParser* parser = new XmlParser();
	parser->ParseXmlFile(FRAMEWORK->GetFileSystemUtils()->GetDeviceBaseResourcesPath() + levelListXmlPath);

	XmlTree* xmlTree = parser->GetXmlTree();
	ASSERT(xmlTree != nullptr, "Got valid xmlTree from parser for level file %s", levelListXmlPath);
	
	XmlNode* rootLevelListNode = xmlTree->GetRootNode();
	ASSERT(rootLevelListNode != nullptr, "Got valid tutoral node from xml tree for tutorial file %s", levelListXmlPath);


	for(XmlNode* missionNode : rootLevelListNode->GetChildren()) {
		FRAMEWORK->GetLogger()->dbglog("\n Loaded mission data for game");
		for(XmlNode* levelDataNode : missionNode->GetChildren()) {
			LevelData data;
			data.name = levelDataNode->GetAttributeValueS(NAME_PROPERTY);
			data.path = levelDataNode->GetAttributeValueS(PATH_PROPERTY);
			data.type = LevelLoader::stringToLevelType(levelDataNode->GetAttributeValueS(TYPE_PROPERTY));
			data.hasTutorial = std::find(levelsWithTutorials.begin(), levelsWithTutorials.end(), data.name) != levelsWithTutorials.end();
			levelData->push_back(data);
		}
	}
	delete parser;
}
void LevelLoader::LoadTutorialLevelNames(std::vector<std::string>* levelsWithTutorials) {
	FRAMEWORK->GetLogger()->dbglog("\nLoading tutorials from file: %s", tutorialXmlPath);

	XmlParser* parser = new XmlParser();
	parser->ParseXmlFile(FRAMEWORK->GetFileSystemUtils()->GetDeviceBaseResourcesPath() + tutorialXmlPath);

	XmlTree* xmlTree = parser->GetXmlTree();
	ASSERT(xmlTree != nullptr, "Got valid xmlTree from parser for level file %s", tutorialXmlPath);
	
	XmlNode* rootTutorialsNode = xmlTree->GetRootNode();
	ASSERT(rootTutorialsNode != nullptr, "Got valid tutoral node from xml tree for tutorial file %s", tutorialXmlPath);

	for(XmlNode* tutorialNode : rootTutorialsNode->GetChildren()) {
		FRAMEWORK->GetLogger()->dbglog("\n Loaded tutorial data for level: %s", tutorialNode->GetAttributeValueS(NAME_ATTRIBUTE).c_str());
		levelsWithTutorials->push_back(tutorialNode->GetAttributeValueS(NAME_ATTRIBUTE));
	}
	delete parser;

}

void LevelLoader::LoadTutorialData(std::string levelName) {
	MessageChunk createTutorialMesssage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	createTutorialMesssage->SetMessageType(MESSAGE_TYPE_CREATED_TUTORIAL);
	createTutorialMesssage->messageData.s = levelName;
	ENGINE->GetMessageHub()->SendMulticastMessage(createTutorialMesssage);

}


void LevelLoader::loadStaticDataFromXml(XmlNode* staticNode) {
	XmlNode* staticObjNode = staticNode->GetFirstChildByNodeName(STATIC_OBJECT_TAG);
	while (staticObjNode != nullptr) {
		int objXmlId       = staticObjNode->GetAttributeValueI(ID_ATTRIBUTE);
		std::string prefab = staticObjNode->GetAttributeValueS(PREFAB_ATTRIBUTE);
		int westBound      = staticObjNode->GetAttributeValueI(X_ATTRIBUTE);
		float yPosition    = staticObjNode->GetAttributeValueF(Y_ATTRIBUTE);
		int southBound     = staticObjNode->GetAttributeValueI(Z_ATTRIBUTE);
		int objWidth       = staticObjNode->GetAttributeValueI(WIDTH_ATTRIBUTE);
		int objHeight      = staticObjNode->GetAttributeValueI(HEIGHT_ATTRIBUTE);
		float rotation     = staticObjNode->GetAttributeValueF(ROTATION_ATTRIBUTE) inRadians;

		GameObject* staticObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + prefab + PREFAB_EXTENSION, ENGINE->GetSceneGraph3D());
		idsFromXml[objXmlId] = staticObj->GetId();

		// Add the object to the grid
		SINGLETONS->GetGridManager()->placeStaticObjectOnGrid(staticObj->GetId(), westBound, southBound, objWidth, objHeight);

		staticObj->GetTransform()->Translate(yPosition, YAXIS);
		staticObj->GetTransform()->SetOrientation(rotation, YAXIS);

		staticObjNode = staticObjNode->GetNextSiblingByNodeName(STATIC_OBJECT_TAG);
	}
}

void LevelLoader::loadUnitDataFromXml(XmlNode* unitBaseNode) {
	// Start with player units
	XmlNode* unitNode = unitBaseNode->GetFirstChildByNodeName("");
	while (unitNode != nullptr) {
		int unitXmlId          = unitNode->GetAttributeValueI(ID_ATTRIBUTE);
		std::string unitPrefab = unitNode->GetAttributeValueS(PREFAB_ATTRIBUTE);
		GameObject* unitObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + unitPrefab + PREFAB_EXTENSION, ENGINE->GetSceneGraph3D());
		idsFromXml[unitXmlId] = unitObj->GetId();

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
	XmlNode* dynamicObjNode = otherDataNode->GetFirstChildByNodeName(DYNAMIC_OBJECT_TAG);
	while (dynamicObjNode != nullptr) {
		int objXmlId       = dynamicObjNode->GetAttributeValueI(ID_ATTRIBUTE);
		std::string prefab = dynamicObjNode->GetAttributeValueS(PREFAB_ATTRIBUTE);
		int xGridPosition  = dynamicObjNode->GetAttributeValueI(X_ATTRIBUTE);
		float yPosition    = dynamicObjNode->GetAttributeValueF(Y_ATTRIBUTE);
		int zGridPosition  = dynamicObjNode->GetAttributeValueI(Z_ATTRIBUTE);
		float rotation     = dynamicObjNode->GetAttributeValueF(ROTATION_ATTRIBUTE) inRadians;

		GameObject* dynamicObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + prefab + PREFAB_EXTENSION, ENGINE->GetSceneGraph3D());
		idsFromXml[objXmlId] = dynamicObj->GetId();

		// Position and orient the object
		dynamicObj->GetTransform()->SetPosition(xGridPosition, yPosition, -zGridPosition);
		dynamicObj->GetTransform()->SetOrientation(rotation, YAXIS);

		// Check for overloaded components
		XmlNode* compNode = dynamicObjNode->GetFirstChildByNodeName(COMPONENT_TAG);
		while (compNode != nullptr) {
			PrefabLoader::LoadComponentFromComponentNodeInPrefab(dynamicObj, compNode);
			compNode = compNode->GetNextSiblingByNodeName(COMPONENT_TAG);
		}

		dynamicObjNode = dynamicObjNode->GetNextSiblingByNodeName(DYNAMIC_OBJECT_TAG);
	}

	// TODO [Implement] Other things such as triggerables
}

void LevelLoader::loadCameraDataFromXml(XmlNode* cameraNode) {
	// The level data file will specify which unit the camera should focus on by default
	std::string unitNameStr = cameraNode->GetAttributeValueS(FOCUS_ATTRIBUTE);

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
	//SINGLETONS->GetGridManager()->selectedUnitId = id;
	PlayerUnit* pU = ENGINE->GetSceneGraph3D()->GetGameObjectById(id)->GetComponent<PlayerUnit>();
	// TODO [Implement]: call this when the level starts so the player sees the selection animation
	//pU->onSelectedTouch();
	SINGLETONS->GetGridManager()->selectUnitInCell(pU->gridNavRef->GetCurrentCell());
	// Set the camera's position
	GameObject* target = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
	GridNavigator* gNav = target->GetComponent<GridNavigator>();
	ASSERT(gNav != nullptr, "Object with id %d has GridNavigator component", id);

	GridCell* cell = gNav->GetCurrentCell();
	ASSERT(cell != nullptr, "Object with id %d is on grid", id);

	// load in start and overview pos
	glm::vec3 startPos;
	bool startPosNodeIsNull = true;
	XmlNode* startNode = cameraNode->GetFirstChildByNodeName(START_TAG);
	if (startNode != nullptr) {
		float startX = startNode->GetAttributeValueF(X_ATTRIBUTE);
		float startY = startNode->GetAttributeValueF(Y_ATTRIBUTE);
		float startZ = startNode->GetAttributeValueF(Z_ATTRIBUTE);
		startPos = glm::vec3(startX, startY, startZ);
		startPosNodeIsNull = false;
	}

	glm::vec3 overviewPos;
	bool overviewNodeIsNull = true;
	XmlNode* overviewNode = cameraNode->GetFirstChildByNodeName(OVERVIEW_TAG);
	if (overviewNode != nullptr) {
		float overviewX = overviewNode->GetAttributeValueF(X_ATTRIBUTE);
		float overviewY = overviewNode->GetAttributeValueF(Y_ATTRIBUTE);
		float overviewZ = overviewNode->GetAttributeValueF(Z_ATTRIBUTE);
		overviewPos = glm::vec3(overviewX, overviewY, overviewZ);
		overviewNodeIsNull = false;
	}
	
	// If a level does not have an overview node something is wrong
	ASSERT(!overviewNodeIsNull, "level XML has information for overview camera");
	cameraComponent->loadCameraData(pU->gridNavRef->GetCurrentCell(), overviewPos, startPos, !startPosNodeIsNull);
}

void LevelLoader::loadLinkDataFromXml  (XmlNode* linkBaseNode) {
	XmlNode* triggerLinkNode = linkBaseNode->GetFirstChildByNodeName(TRIGGER_LINK_TAG);
	while (triggerLinkNode != nullptr) {
		int triggerXmlId = triggerLinkNode->GetAttributeValueI(ID_ATTRIBUTE);
		ObjectIdType triggerId = idsFromXml[triggerXmlId];
		GameObject* triggerObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(triggerId);
		ASSERT(triggerObj != nullptr, "Trigger object with id %d exists in level", triggerId);
		if (triggerObj != nullptr) {
			Triggerable* triggerComp = triggerObj->GetComponent<Triggerable>();
			MultiplexSwitch* multiplexComp = triggerObj->GetComponent<MultiplexSwitch>();
			ASSERT((triggerComp != nullptr) || (multiplexComp != nullptr), "Object with id %d has Triggerable component", triggerId);
			if (triggerComp != nullptr) {
				XmlNode* switchNode = triggerLinkNode->GetFirstChildByNodeName(SWITCH_TAG);
				while (switchNode != nullptr) {
					ObjectIdType switchId = switchNode->GetAttributeValueI(ID_ATTRIBUTE);
					triggerComp->SubscribeToSwitchObject(idsFromXml[switchId]);

					switchNode = switchNode->GetNextSiblingByNodeName(SWITCH_TAG);
				}
			}
			if (multiplexComp != nullptr) {
				XmlNode* switchNode = triggerLinkNode->GetFirstChildByNodeName(SWITCH_TAG);
				while (switchNode != nullptr) {
					ObjectIdType switchId = switchNode->GetAttributeValueI(ID_ATTRIBUTE);
					multiplexComp->SubscribeToSwitchObject(idsFromXml[switchId]);

					switchNode = switchNode->GetNextSiblingByNodeName(SWITCH_TAG);
				}
			}
		}

		triggerLinkNode = triggerLinkNode->GetNextSiblingByNodeName(TRIGGER_LINK_TAG);
	}
}
