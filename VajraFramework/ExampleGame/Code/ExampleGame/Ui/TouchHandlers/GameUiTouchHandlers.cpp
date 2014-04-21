 #include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "ExampleGame/Components/LevelManager/LevelFileTags.h"
#include "ExampleGame/Components/LevelManager/LevelLoader.h"
#include "ExampleGame/Messages/Declarations.h"
#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"
#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Engine/Ui/UiCallbackComponent/UiCallbackComponent.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"


#if DEBUG
#include "ExampleGame/Ui/TouchHandlers/DebugMenuTouchHandlers.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#endif

// Todo [HACK] when level loading is better we probably won't need all these
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"

#define IN_GAME_MENU "inGame"
#define PAUSE_MENU "pauseMenu"
#define PRE_GAME_MENU "preMenu"
#define POST_GAME_WIN_MENU "postWinGame"
#define POST_GAME_LOSE_MENU "postLoseGame"
#define TUTORIAL_MENU "tutorialScreen"

#define TUTORIAL_EXIT_BTN "closeTutorial"
#define DYNAMIC_TUTORIAL_ELEMENT "dynamicTutorial"
#define ASSASSIN_ICON_INDEX 0
#define THIEF_ICON_INDEX 1

void onTutorialTweenInComplete(ObjectIdType gameObjectId, std::string /* tweenClipName */) {
	UiObject* tutorialMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(gameObjectId);
	ASSERT(tutorialMenu != nullptr, "Tutorial menu object is not-null in onTutorialTweenInComplete");
	if(tutorialMenu != nullptr) {
		tutorialMenu->SetClickable(true);
	}

}

void onTutorialTweenOutComplete(ObjectIdType gameObjectId, std::string /* tweenClipName */) {
	UiObject* tutorialMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(gameObjectId);
	ASSERT(tutorialMenu != nullptr, "Tutorial menu object is not-null in onTutorialTweenOutComplete");
	if(tutorialMenu != nullptr) {
		tutorialMenu->SetClickable(false);
	}
	
	Object* dynamicTutorial = ObjectRegistry::GetObjectByName(DYNAMIC_TUTORIAL_ELEMENT);
	if(dynamicTutorial != nullptr) {
		delete dynamicTutorial;
		dynamicTutorial = nullptr;
	}
}

GameUiTouchHandlers::GameUiTouchHandlers() : UiTouchHandlers() {
	this->isTutorialLevel = false;
	this->dynamicTutorialElement = nullptr;
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_SELECTED_UNIT_CHANGED);
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_CREATED_TUTORIAL);
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_ON_END_CONDITIONS_MET);
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_SCENE_START);
}

GameUiTouchHandlers::~GameUiTouchHandlers() {
	if(this->dynamicTutorialElement != nullptr) {
		delete this->dynamicTutorialElement;
		this->dynamicTutorialElement = nullptr;
	}
}

void GameUiTouchHandlers::HandleMessageCallback(MessageChunk messageChunk) {
	switch(messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_SELECTED_UNIT_CHANGED:
			if(messageChunk->messageData.iv1.y == UNIT_TYPE_ASSASSIN) {
				UiElement* changeUnitIcon = (UiElement*)ObjectRegistry::GetObjectByName("changeUnit");
				ASSERT(changeUnitIcon != nullptr, "chagne unit icon found");
				ASSERT(changeUnitIcon->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a ui element");
				changeUnitIcon->SetSpriteTextureIndex(ASSASSIN_ICON_INDEX);

			} else if(messageChunk->messageData.iv1.y == UNIT_TYPE_THIEF) {
				UiElement* changeUnitIcon = (UiElement*)ObjectRegistry::GetObjectByName("changeUnit");
				ASSERT(changeUnitIcon != nullptr, "chagne unit icon found");
				ASSERT(changeUnitIcon->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a ui element");
				changeUnitIcon->SetSpriteTextureIndex(THIEF_ICON_INDEX);
			}
			break;
		case MESSAGE_TYPE_CREATED_TUTORIAL:
			this->setupTutorial(messageChunk->messageData.s);
			break;
		case MESSAGE_TYPE_ON_END_CONDITIONS_MET:
			this->onLevelEnd(messageChunk->messageData.iv1.x >= 0);
			break;
		default:
			break;
	}
	
	if(this->isTutorialLevel) {
		// for each tutorial screen in this level see if it should occur
		for(unsigned int i = 0; i < this->tutorials.size(); ++i) {
			if(!this->tutorials[i].hasFired) {
				this->tryTutorial(i, messageChunk);
			}
		}
	}

}

void GameUiTouchHandlers::OnTouchDownHandlers(UiObject* uiObject, Touch /* touch */) {
	if (uiObject->GetName() == "play") {
		// Do something

	} else {
		// Do something

	}

}

void GameUiTouchHandlers::OnTouchMoveHandlers(UiObject* uiObject, Touch /* touch */) {

	if (uiObject->GetName() == "play") {
		// Do something

	} else {
		// Do something

	}

}

void GameUiTouchHandlers::OnTouchUpHandlers(UiObject* uiObject, Touch /* touch */) {
#ifdef DEBUG 
	if (uiObject->GetName() == "debugMenu") {
		std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "debugMenu.uiscene";
		//
		UiSceneLoader::UnloadCurrentUiScene();
		UiSceneLoader::LoadUiSceneFromUiSceneFile(pathToTestUiScene.c_str(), new DebugMenuTouchHandlers());
		// Return immediately here because the current scene has been unloaded
		return;
	}
#endif
	// PRE MENU
	UiObject* preMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[PRE_GAME_MENU]);
	if(preMenu) {
		if(uiObject->GetName() == "preMenuStart") {
			UiElement* preMenuBackground = (UiElement*)ObjectRegistry::GetObjectByName("preMenu");
			SINGLETONS->GetMenuManager()->TweenOutUiObject(preMenuBackground);
			SINGLETONS->GetLevelManager()->StartLevel();

		}
	}

	// PAUSE MENU
	UiObject* pauseMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[PAUSE_MENU]);
	if(pauseMenu->IsVisible()) {
		if (uiObject->GetName() == "resume") {
			pauseMenu->SetVisible(false);
			ENGINE->GetSceneGraph3D()->Resume();
			UiElement* pauseButton = (UiElement*)ObjectRegistry::GetObjectByName("pause");
			pauseButton->SetSpriteTextureIndex(0);
			return;
		} else if (uiObject->GetName() == "restart_pause") {
			pauseMenu->SetVisible(!pauseMenu->IsVisible());
			this->tutorials.clear();
			SINGLETONS->GetMenuManager()->LoadLevel(SINGLETONS->GetLevelManager()->GetCurrentLevelIndex());
			UiElement* pauseButton = (UiElement*)ObjectRegistry::GetObjectByName("pause");
			pauseButton->SetSpriteTextureIndex(0);
			return;
		} else if (uiObject->GetName() == "mission_from_pause") {
			pauseMenu->SetVisible(false);
			this->returnToMissionSelect();
			return;
		}
	}
	
	// HUD
	if (uiObject->GetName() == "pause") {
		pauseMenu->SetVisible(!pauseMenu->IsVisible());
		if(pauseMenu->IsVisible()) {
			((UiElement*)uiObject)->SetSpriteTextureIndex(1);
			ENGINE->GetSceneGraph3D()->Pause();
			this->UpdateMenuWithMastery(PAUSE_MENU, SINGLETONS->GetLevelManager()->GetCurrentLevelIndex());
		} else {
			ENGINE->GetSceneGraph3D()->Resume();
			((UiElement*)uiObject)->SetSpriteTextureIndex(0);
		}
		return;
	} else if(uiObject->GetName() == "changeUnit") {
		SINGLETONS->GetGridManager()->SwitchSelectedUnit();
		return;
	}
	
	// END MENU
	UiObject* postMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[POST_GAME_WIN_MENU]);
	//ASSERT(postMenu != nullptr, "postMenu still around");
	if(postMenu != nullptr && !postMenu->IsVisible()) {
		postMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[POST_GAME_LOSE_MENU]);
	}
	if(postMenu != nullptr && postMenu->IsVisible()) {
		if (uiObject->GetName() == "continue") {
			postMenu->SetVisible(false);
			SINGLETONS->GetMenuManager()->LoadLevel(SINGLETONS->GetLevelManager()->GetCurrentLevelIndex() + 1);
			return;
		} else if (uiObject->GetName() == "restart_postWin" || uiObject->GetName() == "restart_postLose") {
			postMenu->SetVisible(false);
			this->tutorials.clear();
			SINGLETONS->GetMenuManager()->LoadLevel(SINGLETONS->GetLevelManager()->GetCurrentLevelIndex());
			return;
		} else if (uiObject->GetName() == "mission_postWin" || uiObject->GetName() == "mission_postLose") {
			postMenu->SetVisible(false);
			this->returnToMissionSelect();
			return;
		}
	}	

	// TUTORIAL;
	if(uiObject->GetName() == TUTORIAL_EXIT_BTN) {
		this->nextTutorialImage();
		return;
	}
}

void GameUiTouchHandlers::returnToMissionSelect() {
	SINGLETONS->GetMenuManager()->LoadMainMenu(PARALLAX);
	
}

MessageType stringToMessageType(std::string msgString) {
	if(msgString == "MESSAGE_TYPE_GRID_ROOM_ENTERED") {
		return MESSAGE_TYPE_GRID_ROOM_ENTERED;
	} else if (msgString == "MESSAGE_TYPE_GRID_ZONE_ENTERED") {
		return MESSAGE_TYPE_GRID_ZONE_ENTERED;
	} else if(msgString == "MESSAGE_TYPE_SELECTED_UNIT_CHANGED") {
		return MESSAGE_TYPE_SELECTED_UNIT_CHANGED;
	} else if(msgString == "MESSAGE_TYPE_LEVEL_LOADED") {
		return MESSAGE_TYPE_LEVEL_LOADED;
	} else if(msgString == "MESSAGE_TYPE_SCENE_START") {
		return MESSAGE_TYPE_SCENE_START;
	}
	ASSERT(true, "stringToMessageType has reached the end without returning a message. Did you add a case for %s?", msgString.c_str());
	return MESSAGE_TYPE_UNSPECIFIED;
}

void GameUiTouchHandlers::setupTutorial(std::string levelName) {
	// remove the old tutorial data
	this->tutorials.clear();
	
	FRAMEWORK->GetLogger()->dbglog("\nLoading tutorials from file: %s", tutorialXmlPath);

	XmlParser* parser = new XmlParser();
	parser->ParseXmlFile(FRAMEWORK->GetFileSystemUtils()->GetDeviceBaseResourcesPath() + tutorialXmlPath);

	XmlTree* xmlTree = parser->GetXmlTree();
	ASSERT(xmlTree != nullptr, "Got valid xmlTree from parser for level file %s", tutorialXmlPath);
	
	XmlNode* rootTutorialsNode = xmlTree->GetRootNode();
	ASSERT(rootTutorialsNode != nullptr, "Got valid tutoral node from xml tree for tutorial file %s", tutorialXmlPath);

	XmlNode* tutorialNode = nullptr;
	for(XmlNode* node : rootTutorialsNode->GetChildren()) {
		FRAMEWORK->GetLogger()->dbglog("\n Loaded tutorial data for level: %s", node->GetAttributeValueS(NAME_ATTRIBUTE).c_str());
		if(levelName == node->GetAttributeValueS(NAME_ATTRIBUTE)) {
			tutorialNode = node;
			break;
		}
	}
	ASSERT(tutorialNode != nullptr, "Tutorial node of level passed via 'MakeTutorial' does not have a tutorial in the xml");
	if(tutorialNode != nullptr) {
		this->isTutorialLevel = true;
		// load in the tutorial
		XmlNode* messageNode = tutorialNode->GetFirstChildByNodeName(MESSAGE_TAG);
		while(messageNode != nullptr) {
			ASSERT(messageNode != nullptr, "Tutorial node does not have a message node");
			TutorialData tData;
			MessageType tutorialMessageType = stringToMessageType(messageNode->GetAttributeValueS(NAME_ATTRIBUTE));
			this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(tutorialMessageType);
			tData.msgType = tutorialMessageType;
			
			// Load in data needed for this specific tutorial
			XmlNode* cell;
			switch(tutorialMessageType) {
				case MESSAGE_TYPE_GRID_CELL_CHANGED:
					cell = messageNode->GetFirstChildByNodeName(CELL_DATA_TAG);
					tData.vector3Data = glm::vec3(cell->GetAttributeValueI(X_ATTRIBUTE), 0.0f, cell->GetAttributeValueI(Z_ATTRIBUTE));
					break;
				case MESSAGE_TYPE_GRID_ROOM_ENTERED:
					cell = messageNode->GetFirstChildByNodeName(CELL_DATA_TAG);
					tData.vector3Data = SINGLETONS->GetGridManager()->GetGrid()->GetRoomCenter(cell->GetAttributeValueI(X_ATTRIBUTE),
									    cell->GetAttributeValueI(Z_ATTRIBUTE));
					break;
				default:
					break;

			}
			
			// Load in the images
			XmlNode* imageNode = messageNode->GetFirstChildByNodeName(IMAGE_TAG);
			while(imageNode != nullptr) {
				std::string imageName = imageNode->GetAttributeValueS(NAME_ATTRIBUTE);
				tData.imageNames.push_back(imageName);
				imageNode = imageNode->GetNextSiblingByNodeName(IMAGE_TAG);
			}
			
			tData.hasFired = false;
			this->tutorials.push_back(tData);
			
			messageNode = messageNode->GetNextSiblingByNodeName(MESSAGE_TAG);										 
		}
	}
	delete parser;
}

void GameUiTouchHandlers::tryTutorial(int index, MessageChunk messageChunk) {
	if(this->tutorials[index].msgType != messageChunk->GetMessageType()) {
		return;
	}
	
	switch(this->tutorials[index].msgType) {
		// TODO [Implement] this message type when it's needed
		/*
		case MESSAGE_TYPE_GRID_ZONE_ENTERED:
			
			if(this->tutorials[index].vector3Data != messageChunk->messageData.fv1) {
				//return;
			}
			break;
		*/
		case MESSAGE_TYPE_GRID_ROOM_ENTERED:
			if(this->tutorials[index].vector3Data != messageChunk->messageData.fv1) {
				return;
			}
			break;
		default:
			break;
	}

	this->currentTutorialIndex = index;
	this->tutorials[index].hasFired = true; // we do not want the tutorial opening twice
	
	// Grab
	UiObject* tut = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[TUTORIAL_MENU]);
	tut->SetClickable(false);

	// Delete the old tutorial if it is still around
	if(this->dynamicTutorialElement != nullptr) {
		this->dynamicTutorialElement->SetParent(ENGINE->GetSceneGraph3D()->GetRootGameObject()->GetId());
		delete this->dynamicTutorialElement;
		this->dynamicTutorialElement = nullptr;
	}
	
	// Load the textures for the tutorial
	this->dynamicTutorialElement = new UiElement(ENGINE->GetSceneGraphUi());
	tut->AddChild(this->dynamicTutorialElement->GetId());
	this->dynamicTutorialElement->SetName(DYNAMIC_TUTORIAL_ELEMENT);
	if (this->tutorials[index].imageNames.size() != 0) {
		std::vector<std::string> imagePaths;
		for (std::string imageName : this->tutorials[index].imageNames) {
			imagePaths.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + imageName);
		}

		this->dynamicTutorialElement->GetTransform()->SetPosition(0.5f, -34.0f, this->dynamicTutorialElement->GetTransform()->GetPosition().z);
		this->dynamicTutorialElement->InitSprite(768, 432, "ustshdr", imagePaths, false);
		this->dynamicTutorialElement->SetZOrder(3);
		UiElement* exitBtn = (UiElement*)ObjectRegistry::GetObjectByName(TUTORIAL_EXIT_BTN);
		
		if (this->tutorials[index].imageNames.size() == 1) {
			exitBtn->SetSpriteTextureIndex(0);
		} else {
			exitBtn->SetSpriteTextureIndex(1);
		}
	} 
	this->dynamicTutorialElement->SetVisible(true);
	tut->SetVisible(true);
	// tween in the tutorial
	SINGLETONS->GetMenuManager()->TweenInUiObject(tut);
}

void GameUiTouchHandlers::nextTutorialImage() {
	unsigned int textureIndex = this->dynamicTutorialElement->GetSpriteTextureIndex();
	textureIndex++;
	if(textureIndex == this->tutorials[this->currentTutorialIndex].imageNames.size()) {
		UiObject* tut = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[TUTORIAL_MENU]);
		SINGLETONS->GetMenuManager()->TweenOutUiObject(tut);
		return;
	}
	ASSERT(textureIndex < this->tutorials[this->currentTutorialIndex].imageNames.size(), "nextTutorialImage() has been called when the tutorial is out of images to show. ");
	this->dynamicTutorialElement->SetSpriteTextureIndex(textureIndex);
	if(textureIndex + 1 == this->tutorials[this->currentTutorialIndex].imageNames.size()) {
		UiElement* exitBtn = (UiElement*)ObjectRegistry::GetObjectByName(TUTORIAL_EXIT_BTN);
		exitBtn->SetSpriteTextureIndex(0);

	} else if(textureIndex == this->tutorials[this->currentTutorialIndex].imageNames.size()) {
		UiObject* tut = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[TUTORIAL_MENU]);
		SINGLETONS->GetMenuManager()->TweenOutUiObject(tut);
	}
}

void GameUiTouchHandlers::onLevelEnd(bool success) {
	ENGINE->GetSceneGraph3D()->Pause();
	UiObject* postMenu;
	if(success){
		 postMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[POST_GAME_WIN_MENU]);
		 UpdateMenuWithMastery(POST_GAME_WIN_MENU, SINGLETONS->GetLevelManager()->GetCurrentLevelIndex());

	} else {
		 postMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[POST_GAME_LOSE_MENU]);
	}
	postMenu->SetVisible(true);
	SINGLETONS->GetMenuManager()->TweenInUiObject(postMenu);
	this->tutorials.clear();
}

void GameUiTouchHandlers::UpdateMenuWithMastery(std::string menuName, int levelIndex) {
	std::string menuPrefix;
	if(menuName == POST_GAME_WIN_MENU) {
		menuPrefix = "postGame_";
	} else if( menuName == PRE_GAME_MENU) {
		menuPrefix = "preGame_";
	} else if(menuName == PAUSE_MENU){
		menuPrefix = "pause_";
	} else {
		ASSERT(false, "\n %s does not support the mastery system", menuName.c_str());
		return;
	}
	LevelScores scores = SINGLETONS->GetMasteryManager()->GetLevelScores(levelIndex);
	UiElement* menuRoot = (UiElement*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[menuName]);
	for(ObjectIdType id : menuRoot->GetChildren()) {
		UiElement* child = (UiElement*)ENGINE->GetSceneGraphUi()->GetGameObjectById(id);
		if(child->GetName() == menuPrefix + "bonus_value") {
			child->ChangeText("Testeroini");
		} else if (child->GetName() == menuPrefix + "bounty_value") {
			child->ChangeText("Testeroini");
		} else if (child->GetName() == menuPrefix + "take_value") {
			child->ChangeText("Testeroini");
		} else if (child->GetName() == menuPrefix + "bonus_completion") {
			child->ChangeText("INCOMPLETE");
		} else if (child->GetName() == menuPrefix + "time_value") {
			int time = SINGLETONS->GetMasteryManager()->GetLevelTime();
			child->ChangeText(std::to_string(time));
		} else if (child->GetName() == menuPrefix + "time_total") {
			child->ChangeText(":30 x 5 = -500");
		} else if (child->GetName() == menuPrefix + "kill_value") {
			int kills = SINGLETONS->GetMasteryManager()->GetNumKills();
			child->ChangeText(std::to_string(kills));
		} else if (child->GetName() == menuPrefix + "kill_total") {
			child->ChangeText("4 x 7 = -1400");
		} else if (child->GetName() == menuPrefix + "alert_value") {
			int alerts = SINGLETONS->GetMasteryManager()->GetNumAlerts();
			child->ChangeText(std::to_string(alerts));
		} else if (child->GetName() == menuPrefix + "alert_total") {
			child->ChangeText("3 x -50 = -150");
		} else if (child->GetName() == menuPrefix + "loot_value") {	
			int loot = SINGLETONS->GetMasteryManager()->GetMoney();
			child->ChangeText(std::to_string(loot));
		} else if (child->GetName() == menuPrefix + "loot_total") {
			int loot = SINGLETONS->GetMasteryManager()->GetMoney();
			child->ChangeText(std::to_string(loot));
		} else if (child->GetName() == menuPrefix + "time_score") {
			std::string text;
			if(scores.time != -1) {
				text = std::to_string(scores.time);
			} else {
				text = "N/A";
			}
			child->ChangeText(text);
		} else if (child->GetName() == menuPrefix + "kill_score") {
			std::string text;
			if(scores.kills != -1) {
				text = std::to_string(scores.kills);
			} else {
				text = "N/A";
			}
			child->ChangeText(text);
		} else if (child->GetName() == menuPrefix + "alert_score") {
			std::string text;
			if(scores.alerts != -1) {
				text = std::to_string(scores.alerts);
			} else {
				text = "N/A";
			}
			child->ChangeText(text);
		} else if (child->GetName() == menuPrefix + "loot_score") {
			std::string text;
			if(scores.money != -1) {
				text = std::to_string(scores.money);
			} else {
				text = "N/A";
			}
			child->ChangeText(text);
		} 
	}

}