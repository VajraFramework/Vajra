//
//  LevelLoader.cpp
//  Created by Matt Kaufmann on 01/24/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Ai/AiPerception.h"
#include "ExampleGame/Components/GameScripts/Ai/AiRoutine.h"
#include "ExampleGame/Components/GameScripts/Units/EnemyUnit.h"
#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/Grid/GridConstants.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/LevelManager/LevelFileTags.h"
#include "ExampleGame/Components/LevelManager/LevelLoader.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/Components/LevelManager/MasteryManager.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/Components/Switches/BaseSwitch.h"
#include "ExampleGame/Components/Switches/MultiplexSwitch.h"
#include "ExampleGame/Components/Triggers/Triggerable.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "ExampleGame/Ui/MenuManager/MenuDefinitions.h"
#include "ExampleGame/Ui/MenuManager/MenuManager.h"

#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioListener.h"
#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioSource.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Lighting/AmbientLighting.h"
#include "Vajra/Engine/Lighting/ShadowMap.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/Prefabs/PrefabLoader.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/ParserTags.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/SavedDataManager/SavedDataManager.h"
#include "Vajra/Framework/Settings/Settings.h"
#include "Vajra/Utilities/StringUtilities.h"

std::map<int, ObjectIdType> LevelLoader::idsFromXml;

#define DEFAULT_LOSE_CONDITION "DefaultLoseCondition"

UnitType StringToUnitType(std::string str) {
	if (str == "Assassin") {
		return UNIT_TYPE_ASSASSIN;
	} else if (str == "Thief") {
		return UNIT_TYPE_THIEF;
	}
	return UNIT_TYPE_UNKNOWN;
}

LevelBonus StringToLevelBonus(std::string str) {
	if(str == "Time") {
		return LevelBonus::Time;
	} else if(str == "Kills") {
		return LevelBonus::Kills;
	} else if(str == "Money") {
		return LevelBonus::Money;
	} else if(str == "Alerts"){
		return LevelBonus::Alerts;
	} else {
		return LevelBonus::None;
	}
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
	
	std::string bgm = levelNode->GetAttributeValueS(BGM_ATTRIBUTE);
	if (bgm != "") {
		AudioSource* bgmSource = SINGLETONS->GetMenuManager()->GetBGMSource();
		bgmSource->LoadAudioClip(LEVEL_BGM, "audio/" + bgm);
	}

	XmlNode* gridNode = levelNode->GetFirstChildByNodeName(GRID_TAG);
	VERIFY(gridNode != nullptr, "Level definition contains <%s> node", GRID_TAG);
	SINGLETONS->GetGridManager()->loadGridDataFromXml(gridNode);

	// Create the shady camera before we load any objects because their AudioSource components will need it
	createCamera();

	XmlNode* staticNode = levelNode->GetFirstChildByNodeName(STATIC_TAG);
	// The level doesn't need to have a STATIC_TAG node
	if (staticNode != nullptr) {
		loadStaticDataFromXml(staticNode);
	}

	XmlNode* otherDataNode = levelNode->GetFirstChildByNodeName(OTHER_TAG);
	// The level doesn't need to have an OTHER_TAG node
	if (otherDataNode != nullptr) {
		loadOtherDataFromXml(otherDataNode);
	}

	XmlNode* unitBaseNode = levelNode->GetFirstChildByNodeName(UNITS_TAG);
	VERIFY(unitBaseNode != nullptr, "Level definition contains <%s> node", UNITS_TAG);
	loadUnitDataFromXml(unitBaseNode);

	XmlNode* linkBaseNode = levelNode->GetFirstChildByNodeName(LINKS_TAG);
	VERIFY(linkBaseNode != nullptr, "Level definition contains <%s> node", LINKS_TAG);
	if (linkBaseNode != nullptr) {
		loadLinkDataFromXml(linkBaseNode);
		loadParentConnectionsFromXml(linkBaseNode);
		loadEndConditionsFromXml(linkBaseNode);
	}

	XmlNode* cameraNode = levelNode->GetFirstChildByNodeName(CAMERA_TAG);
	VERIFY(cameraNode != nullptr, "Level definition contains <%s> node", CAMERA_TAG);
	loadCameraDataFromXml(cameraNode);

	delete parser;

	SINGLETONS->GetMenuManager()->PlayBGM(LEVEL_BGM);

	idsFromXml.clear();
	
	LevelLoader::adjustLighting();

	LevelLoader::postLoadLevel();
}

void LevelLoader::postLoadLevel() {
	ENGINE->GetSceneGraph3D()->CreateRenderBatches();
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

LevelCompletion LevelLoader::charToCompletionData(char type) {
	if(type == 'U') {
		return LevelCompletion::Unlocked;
	}
	else if(type == 'L') {
		return LevelCompletion::Locked;
	}
	else if(type == 'C') {
		return LevelCompletion::Completed;
	}
	else if(type == 'B') {
		return LevelCompletion::Completed_Bonus;
	}
	ASSERT(0, "%s is a valid completion type", type);
	return LevelCompletion::Locked;
}
void LevelLoader::LoadLevelData(std::vector<ContractData*>* contractData) {
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

	// Get the saved data
	VERIFY(FRAMEWORK->GetSavedDataManager()->HasBundle(PLAYER_BUNDLE_NAME), "The game has saved data");
	Bundle* bundle = FRAMEWORK->GetSavedDataManager()->GetSavedBundle(PLAYER_BUNDLE_NAME);
	ASSERT(bundle->HasKey(LEVEL_COMPLETION), "The saved data has level completion");
	std::string levelCompletionData = bundle->GetString(LEVEL_COMPLETION);

	int missionNum = 0;
	int levelNum = 0;
	for(XmlNode* contractNode : rootLevelListNode->GetChildren()) {
		ContractData* cData = new ContractData();
		cData->name = StringUtilities::ReplaceCharInString(contractNode->GetAttributeValueS("title"), '_', ' ');
		for(XmlNode* missionNode : contractNode->GetChildren()) {
			FRAMEWORK->GetLogger()->dbglog("\n Loaded mission data for game");
			MissionData* mData = new MissionData();
			mData->name = StringUtilities::ReplaceCharInString(missionNode->GetAttributeValueS("title"), '_', ' ');
			for(XmlNode* levelDataNode : missionNode->GetChildren()) {
				LevelData* lData = new LevelData();
				lData->name = levelDataNode->GetAttributeValueS(NAME_PROPERTY);
				lData->name = StringUtilities::ReplaceCharInString(lData->name, '_', ' ');
				lData->path = levelDataNode->GetAttributeValueS(PATH_PROPERTY);
				lData->type = LevelLoader::stringToLevelType(levelDataNode->GetAttributeValueS(TYPE_PROPERTY));
				lData->bounty = StringUtilities::ConvertStringToInt(levelDataNode->GetAttributeValueS("bounty"));
				lData->description = StringUtilities::StringToUpper(levelDataNode->GetAttributeValueS(TYPE_PROPERTY));
				lData->completion = LevelLoader::charToCompletionData(levelCompletionData[levelNum]);
				lData->hasTutorial = std::find(levelsWithTutorials.begin(), levelsWithTutorials.end(), lData->path) != levelsWithTutorials.end();
				lData->levelNum = levelNum;
				lData->bonus = LevelBonus::None;
				for(XmlNode* child : levelDataNode->GetChildren()) {
					if(child->GetName() == "missionScreen") {
						lData->pinX = child->GetAttributeValueF("x");
						lData->pinY = child->GetAttributeValueF("y");
						lData->parallaxScreen = child->GetAttributeValueF("parallaxScreen");
					} else if(child->GetName() == "bonus") {
						lData->bonus = StringToLevelBonus(child->GetAttributeValueS("type"));
						lData->bonusValue = child->GetAttributeValueI("value");
					}
				}
				mData->levels.push_back(lData);
				levelNum++;
			}
			cData->missions.push_back(mData);
			missionNum++;
		}
		contractData->push_back(cData);
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
	/*
	MessageChunk createTutorialMesssage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	createTutorialMesssage->SetMessageType(MESSAGE_TYPE_CREATED_TUTORIAL);
	createTutorialMesssage->messageData.s = levelName;
	ENGINE->GetMessageHub()->SendMulticastMessage(createTutorialMesssage);
	*/
	SINGLETONS->GetMenuManager()->CreateTutorial(levelName);

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

		GridZone* zone = unitObj->GetComponent<GridZone>();
		if (zone != nullptr) {
			SINGLETONS->GetGridManager()->placeZoneOnGrid(unitObj->GetId());//GetGrid()->AddGridZone(dynamicObj->GetId());
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

		GridZone* zone = dynamicObj->GetComponent<GridZone>();
		if (zone != nullptr) {
			SINGLETONS->GetGridManager()->placeZoneOnGrid(dynamicObj->GetId());//GetGrid()->AddGridZone(dynamicObj->GetId());
		}

		dynamicObjNode = dynamicObjNode->GetNextSiblingByNodeName(DYNAMIC_OBJECT_TAG);
	}

	// TODO [Implement] Other things such as triggerables
}

void LevelLoader::loadCameraDataFromXml(XmlNode* cameraNode) {
	// The level data file will specify which unit the camera should focus on by default
	std::string unitNameStr = cameraNode->GetAttributeValueS(FOCUS_ATTRIBUTE);

	// Find the unit that the camera should focus on
	UnitType uType = StringToUnitType(unitNameStr);
	ObjectIdType id = SINGLETONS->GetGridManager()->GetPlayerUnitIdOfType(uType);
	ASSERT(id != OBJECT_ID_INVALID, "Player unit of type %d exists in level", uType);
	//SINGLETONS->GetGridManager()->selectedUnitId = id;
	PlayerUnit* pU = ENGINE->GetSceneGraph3D()->GetGameObjectById(id)->GetComponent<PlayerUnit>();
	// TODO [Implement]: call this when the level starts so the player sees the selection animation

	// Tell the grid who the selected unit is
	SINGLETONS->GetGridManager()->selectUnitInCell(pU->gridNavRef->GetCurrentCell());
	// Tell the select unit they are selected 
	pU->onSelectedTouch();
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
	SINGLETONS->GetGridManager()->GetShadyCamera()->loadCameraData(pU->gridNavRef->GetCurrentCell(), overviewPos, startPos, !startPosNodeIsNull);
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
			ASSERT((triggerComp != nullptr), "Object with id %d has Triggerable component", triggerId);
			if (triggerComp != nullptr) {
				XmlNode* switchNode = triggerLinkNode->GetFirstChildByNodeName(SWITCH_TAG);
				while (switchNode != nullptr) {
					int switchXmlId = switchNode->GetAttributeValueI(ID_ATTRIBUTE);
					ObjectIdType switchId = idsFromXml[switchXmlId];
					triggerComp->SubscribeToSwitchObject(switchId);

					switchNode = switchNode->GetNextSiblingByNodeName(SWITCH_TAG);
				}
			}
		}

		triggerLinkNode = triggerLinkNode->GetNextSiblingByNodeName(TRIGGER_LINK_TAG);
	}
}

void LevelLoader::loadParentConnectionsFromXml(XmlNode* linkBaseNode) {
	XmlNode* parentNode = linkBaseNode->GetFirstChildByNodeName(PARENT_LINK_TAG);
	while (parentNode != nullptr) {
		int parentXmlId = parentNode->GetAttributeValueI(ID_ATTRIBUTE);
		GameObject* parentObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(idsFromXml[parentXmlId]);
		if (parentObj != nullptr) {
			XmlNode* childNode = parentNode->GetFirstChildByNodeName(CHILD_TAG);
			while (childNode != nullptr) {
				int childXmlId = childNode->GetAttributeValueI(ID_ATTRIBUTE);

				parentObj->AddChild_maintainTransform(idsFromXml[childXmlId]);

				childNode = childNode->GetNextSiblingByNodeName(CHILD_TAG);
			}
		}

		parentNode = parentNode->GetNextSiblingByNodeName(PARENT_LINK_TAG);
	}
}

void LevelLoader::loadEndConditionsFromXml(XmlNode* linkBaseNode) {
	XmlNode* winConditionNode = linkBaseNode->GetFirstChildByNodeName(WIN_CONDITION_TAG);
	//ASSERT(winConditionNode != nullptr, "Level has a win condition");
	while (winConditionNode != nullptr) {
		int switchXmlId = winConditionNode->GetAttributeValueI(ID_ATTRIBUTE);
		ObjectIdType switchId = idsFromXml[switchXmlId];
		SINGLETONS->GetLevelManager()->AddWinCondition(switchId);

		winConditionNode = winConditionNode->GetNextSiblingByNodeName(WIN_CONDITION_TAG);
	}

	XmlNode* loseConditionNode = linkBaseNode->GetFirstChildByNodeName(LOSE_CONDITION_TAG);
	while (loseConditionNode != nullptr) {
		int switchXmlId = loseConditionNode->GetAttributeValueI(ID_ATTRIBUTE);
		ObjectIdType switchId = idsFromXml[switchXmlId];
		SINGLETONS->GetLevelManager()->AddLoseCondition(switchId);

		loseConditionNode = loseConditionNode->GetNextSiblingByNodeName(LOSE_CONDITION_TAG);
	}

	// Generate the default lose condition
	GameObject* defaultLose = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + DEFAULT_LOSE_CONDITION + PREFAB_EXTENSION, ENGINE->GetSceneGraph3D());
	SINGLETONS->GetLevelManager()->AddLoseCondition(defaultLose->GetId());
}

void LevelLoader::createCamera() {
	GameObject* shadyCamObj = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + "ShadyCamera" + PREFAB_EXTENSION, ENGINE->GetSceneGraph3D());
	Camera* cam = shadyCamObj->GetComponent<Camera>();
	VERIFY(cam != nullptr, "Game camera object has Camera component");
	ENGINE->GetSceneGraph3D()->SetMainCameraId(shadyCamObj->GetId());
	SINGLETONS->GetGridManager()->SetShadyCamera((ShadyCamera*)cam);

	AudioListener* listener = shadyCamObj->GetComponent<AudioListener>();
	ASSERT(listener != nullptr, "Game camera object has AudioListener component");
	if (listener != nullptr) {
		listener->SetAsActiveListener();
	}
}

void LevelLoader::adjustLighting() {

	/*
	 * Read in settings:
	 */
	float BRIGHTNESS_MULTIPLIER = 1.0f;
	SettingLevel_t brightness_level = FRAMEWORK->GetSettings()->GetSetting(SETTING_TYPE_brightness);
	switch (brightness_level) {
	case SETTING_LEVEL_off   : BRIGHTNESS_MULTIPLIER = 0.0f; break;
	case SETTING_LEVEL_low   : BRIGHTNESS_MULTIPLIER = 0.3f; break;
	case SETTING_LEVEL_medium: BRIGHTNESS_MULTIPLIER = 0.6f; break;
	case SETTING_LEVEL_high  : BRIGHTNESS_MULTIPLIER = 1.0f; break;
	default                  : BRIGHTNESS_MULTIPLIER = 0.6f; break;
	}

	{
		// Add main directional light:
		GameObject* dlight = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(dlight->GetId());
		DirectionalLight* dlightComponent = dlight->AddComponent<DirectionalLight>();
		dlight->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
		dlight->GetTransform()->Rotate(40.0f inRadians, XAXIS);
		dlight->GetTransform()->Rotate(90.0f inRadians, YAXIS);
		//
		dlightComponent->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
		dlightComponent->SetDiffuseColorInts(255, 194, 194, 255);
		dlightComponent->SetIntensity(0.85f * BRIGHTNESS_MULTIPLIER);
		dlightComponent->SetLightType(MAIN_LIGHT_STRING);
	}

	{
		// Add additional light:
		GameObject* dlight = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(dlight->GetId());
		DirectionalLight* dlightComponent = dlight->AddComponent<DirectionalLight>();
		dlight->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
		dlight->GetTransform()->Rotate(30.0f  inRadians, 1.0f, 0.0f, 0.0f);
		dlight->GetTransform()->Rotate(-45.0f inRadians, 0.0f, 1.0f, 0.0f);
		//
		dlightComponent->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
		dlightComponent->SetDiffuseColorInts(108, 126, 201, 255);
		dlightComponent->SetIntensity(0.45f * BRIGHTNESS_MULTIPLIER);
		dlightComponent->SetLightType(ADDITIONAL_LIGHT_STRING);
	}

	{
		// Add additional light:
		GameObject* dlight = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(dlight->GetId());
		DirectionalLight* dlightComponent = dlight->AddComponent<DirectionalLight>();
		dlight->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
		dlight->GetTransform()->Rotate(30.0f  inRadians, 1.0f, 0.0f, 0.0f);
		dlight->GetTransform()->Rotate(45.0f inRadians, 0.0f, 1.0f, 0.0f);
		//
		dlightComponent->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
		dlightComponent->SetDiffuseColorInts(255, 187, 69, 255);
		dlightComponent->SetIntensity(0.75 * BRIGHTNESS_MULTIPLIER);
		dlightComponent->SetLightType(ADDITIONAL_LIGHT_STRING);
	}

	{
		// Add additional light:
		GameObject* dlight = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(dlight->GetId());
		DirectionalLight* dlightComponent = dlight->AddComponent<DirectionalLight>();
		dlight->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
		dlight->GetTransform()->Rotate(30.0f  inRadians, 1.0f, 0.0f, 0.0f);
		dlight->GetTransform()->Rotate(-135.0f inRadians, 0.0f, 1.0f, 0.0f);
		//
		dlightComponent->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
		dlightComponent->SetDiffuseColorInts(121, 175, 204, 255);
		dlightComponent->SetIntensity(0.65 * BRIGHTNESS_MULTIPLIER);
		dlightComponent->SetLightType(ADDITIONAL_LIGHT_STRING);
	}


	// Set up depth camera properties for real time shadows:
	ENGINE->GetShadowMap()->SetOrthoBounds(-30.0f, 30.0f, -30.0f, 30.0f, 0.0f, 60.0f);
}
