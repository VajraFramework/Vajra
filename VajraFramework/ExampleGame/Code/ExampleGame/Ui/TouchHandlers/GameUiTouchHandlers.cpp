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
#define POST_GAME_MENU "postGame"
#define TUTORIAL_MENU "tutorialScreen"

#define TUTORIAL_EXIT_BTN "closeTutorial"
#define TUTORIAL_NEXT_BTN "nextTutorial"
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
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_SELECTED_UNIT_CHANGED);
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_CREATED_TUTORIAL);
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_ON_END_CONDITIONS_MET);
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
	// PAUSE MENU
	UiObject* pauseMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[PAUSE_MENU]);
	if(pauseMenu->IsVisible()) {
		if (uiObject->GetName() == "resume") {
			pauseMenu->SetVisible(false);
			ENGINE->GetSceneGraph3D()->Resume();
			return;
		} else if (uiObject->GetName() == "restart_pause") {
			pauseMenu->SetVisible(!pauseMenu->IsVisible());
			SINGLETONS->GetMenuManager()->LoadLevel(SINGLETONS->GetLevelManager()->GetCurrentLevelIndex());
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
		ENGINE->GetSceneGraph3D()->Pause();
		return;
	} else if(uiObject->GetName() == "changeUnit") {
		SINGLETONS->GetGridManager()->SwitchSelectedUnit();
		return;
	}
	
	// END MENU
	UiObject* postMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[POST_GAME_MENU]);
	//ASSERT(postMenu != nullptr, "postMenu still around");
	if(postMenu == nullptr) {
		FRAMEWORK->GetLogger()->dbglog("\n TOUCH: %s \n", uiObject->GetName().c_str());
	}
	if(postMenu != nullptr && postMenu->IsVisible()) {
		if (uiObject->GetName() == "continue") {
			postMenu->SetVisible(false);
			SINGLETONS->GetMenuManager()->LoadLevel(SINGLETONS->GetLevelManager()->GetCurrentLevelIndex() + 1);
			return;
		} else if (uiObject->GetName() == "restart_post") {
			postMenu->SetVisible(false);
			SINGLETONS->GetMenuManager()->LoadLevel(SINGLETONS->GetLevelManager()->GetCurrentLevelIndex());
			return;
		} else if (uiObject->GetName() == "mission_post") {
			postMenu->SetVisible(false);
			this->returnToMissionSelect();
			return;
		}
	}	

	// TUTORIAL
	if(uiObject->GetName() == TUTORIAL_NEXT_BTN) {
		this->nextTutorialImage();
		return;
	} else if(uiObject->GetName() == TUTORIAL_EXIT_BTN) {
		// tween in the tutorial
		UiObject* tut = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[TUTORIAL_MENU]);
	
		ENGINE->GetTween()->TweenPosition(tut->GetId(),
										  tut->GetTransform()->GetPosition(),
										  glm::vec3(tut->GetTransform()->GetPosition().x, -768.0f, tut->GetTransform()->GetPosition().z),
										  0.01f,
										  false,
										  TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
										  false,
										  onTutorialTweenOutComplete);
		return;
	}
}

void GameUiTouchHandlers::returnToMissionSelect() {
	SINGLETONS->GetMenuManager()->LoadMainMenu("missionMenu");
	
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
	

	// Load the textures for the tutorial
	this->dynamicTutorialElement = new UiElement(ENGINE->GetSceneGraphUi());
	tut->AddChild(this->dynamicTutorialElement->GetId());
	this->dynamicTutorialElement->SetName(DYNAMIC_TUTORIAL_ELEMENT);
	if (this->tutorials[index].imageNames.size() != 0) {
		std::vector<std::string> imagePaths;
		for (std::string imageName : this->tutorials[index].imageNames) {
			imagePaths.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + imageName);
		}

		this->dynamicTutorialElement->GetTransform()->SetPosition(128.0f, -128.0f, this->dynamicTutorialElement->GetTransform()->GetPosition().z);
		this->dynamicTutorialElement->InitSprite(768, 512, "ustshdr", imagePaths, false);

		UiElement* exitBtn = (UiElement*)ObjectRegistry::GetObjectByName(TUTORIAL_EXIT_BTN);
		UiElement* nextBtn = (UiElement*)ObjectRegistry::GetObjectByName(TUTORIAL_NEXT_BTN);
		
		if (this->tutorials[index].imageNames.size() == 1) {
			exitBtn->SetVisible(true);
			nextBtn->SetVisible(false);
		} else {
			exitBtn->SetVisible(false);
			nextBtn->SetVisible(true);
		}
	}
	this->dynamicTutorialElement->SetVisible(true);
	
	// tween in the tutorial
	ENGINE->GetTween()->TweenPosition(tut->GetId(),
									  tut->GetTransform()->GetPosition(),
									  glm::vec3(tut->GetTransform()->GetPosition().x, 0.0f, tut->GetTransform()->GetPosition().z),
									  0.01f,
									  false,
									  TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
									  false,
									  onTutorialTweenInComplete);
}

void GameUiTouchHandlers::nextTutorialImage() {
	unsigned int textureIndex = this->dynamicTutorialElement->GetSpriteTextureIndex();
	textureIndex++;
	ASSERT(textureIndex < this->tutorials[this->currentTutorialIndex].imageNames.size(), "nextTutorialImage() has been called when the tutorial is out of images to show. ");
	this->dynamicTutorialElement->SetSpriteTextureIndex(textureIndex);
	if(textureIndex + 1 == this->tutorials[this->currentTutorialIndex].imageNames.size()) {
		UiElement* exitBtn = (UiElement*)ObjectRegistry::GetObjectByName(TUTORIAL_EXIT_BTN);
		UiElement* nextBtn = (UiElement*)ObjectRegistry::GetObjectByName(TUTORIAL_NEXT_BTN);
		exitBtn->SetVisible(true);
		exitBtn->SetClickable(true);
		nextBtn->SetVisible(false);

	}
}

void GameUiTouchHandlers::onLevelEnd(bool /*success*/) {
	UiObject* postMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[POST_GAME_MENU]);
	postMenu->SetVisible(true);
}
