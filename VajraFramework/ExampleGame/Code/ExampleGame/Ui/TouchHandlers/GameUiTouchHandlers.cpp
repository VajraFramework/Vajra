 #include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "ExampleGame/Components/LevelManager/LevelFileTags.h"
#include "ExampleGame/Components/LevelManager/LevelLoader.h"
#include "ExampleGame/Components/LevelManager/TutorialCache.h"
#include "ExampleGame/Messages/Declarations.h"
#include "ExampleGame/Ui/MenuManager/MenuDefinitions.h"
#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"
#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Engine/AssetLibrary/Assets/TextureAssets/TextureAsset.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Engine/Ui/UiCallbackComponent/UiCallbackComponent.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
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

#include <memory>

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
	this->init();
}

GameUiTouchHandlers::~GameUiTouchHandlers() {
	this->destroy();
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
		/*
		case MESSAGE_TYPE_ON_END_CONDITIONS_MET:
			this->onLevelEnd(messageChunk->messageData.iv1.x >= 0);
			break;
		*/
		case MESSAGE_TYPE_LEVEL_WON:
			this->onLevelEnd(true);
			break;
		case MESSAGE_TYPE_LEVEL_LOST:
			this->onLevelEnd(false);
			break;
		case MESSAGE_TYPE_OPEN_POST_MENU:
			this->showPostMenu(true);
			break;
		default:
			break;
	}
	
	if(this->isTutorialLevel && !ENGINE->GetSceneGraph3D()->IsPaused()) {
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
	SINGLETONS->GetMenuManager()->PlaySFX(BUTTON_CLICK_SFX);

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
			ENGINE->GetSceneGraph3D()->Resume();

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
			SINGLETONS->GetMenuManager()->UpdateMenuWithMastery(PAUSE_MENU, SINGLETONS->GetLevelManager()->GetCurrentLevelIndex());
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
	if(uiObject->GetName() == DYNAMIC_TUTORIAL_ELEMENT) {
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
	/*
	} else if(msgString == "MESSAGE_TYPE_ON_END_CONDITIONS_MET") {
		return MESSAGE_TYPE_ON_END_CONDITIONS_MET;
	*/
	} else if(msgString == "MESSAGE_TYPE_LEVEL_WON") {
		return MESSAGE_TYPE_LEVEL_WON;
	} else if(msgString == "MESSAGE_TYPE_LEVEL_LOST") {
		return MESSAGE_TYPE_LEVEL_LOST;
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
			
			tData.isCutscene = false;
			if (messageNode->HasAttribute(TYPE_ATTRIBUTE)) {
				std::string tutorialType = messageNode->GetAttributeValueS(TYPE_ATTRIBUTE);
				if (tutorialType == "Cutscene") {
					tData.isCutscene = true;
				}
			}

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
	static unsigned int cachedTutorial = (unsigned int)-1;
	{
		// Find the first tutorial that has not yet fired:
		unsigned int unfiredTutorial = 0;
		for (unfiredTutorial = 0; unfiredTutorial < this->tutorials.size(); ++unfiredTutorial) {
			if(!this->tutorials[unfiredTutorial].hasFired) {
				break;
			}
		}
		if (unfiredTutorial == cachedTutorial || unfiredTutorial >= this->tutorials.size()) {
			// Nothing to do; already cached
		} else {
			// Must cache next tutorial's images:
			if (this->tutorialCache_ref == nullptr) {
				this->tutorialCache_ref = new GameObject(ENGINE->GetSceneGraphUi());
				this->tutorialCache_ref->AddComponent<TutorialCache>();
			}
			if (!this->tutorials[unfiredTutorial].imageNames.empty()) {
				MessageChunk messageChunk = ENGINE->GetMessageHub()->GetOneFreeMessage();
				messageChunk->SetMessageType(MESSAGE_TYPE_PRELOAD_IMAGE);
				messageChunk->messageData.s = this->tutorials[unfiredTutorial].imageNames[0];
				ENGINE->GetMessageHub()->SendPointcastMessage(messageChunk, this->tutorialCache_ref->GetId());
			}
			//
			cachedTutorial = unfiredTutorial;
		}

	}

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

		if (this->tutorials[index].isCutscene) {
			float halfWidth = ((float)tut->GetWidth()) / 2.0f;
			float halfHeight = ((float)tut->GetHeight()) / 2.0f;

			float halfScreenWidth = ((float)FRAMEWORK->GetDeviceProperties()->GetWidthPixels()) / 2.0f;
			float halfScreenHeight= ((float)FRAMEWORK->GetDeviceProperties()->GetHeightPixels()) / 2.0f;

			float xx = halfWidth - halfScreenWidth;
			float yy = halfScreenHeight - halfHeight;
			this->dynamicTutorialElement->GetTransform()->SetPosition(xx, yy, this->dynamicTutorialElement->GetTransform()->GetPosition().z);
			this->dynamicTutorialElement->InitSprite(FRAMEWORK->GetDeviceProperties()->GetWidthPixels(), FRAMEWORK->GetDeviceProperties()->GetHeightPixels(), "ustshdr", imagePaths, false);
		}
		else {
			this->dynamicTutorialElement->GetTransform()->SetPosition(0.5f, -34.0f, this->dynamicTutorialElement->GetTransform()->GetPosition().z);
			int width_out = 768;
			int height_out = 432;
			int dummy = 10;
			UiSceneLoader::AdjustPositionForResolution(dummy, dummy, "LEFT", "TOP", width_out, height_out, 1024, 768);
			this->dynamicTutorialElement->InitSprite(width_out, height_out, "ustshdr", imagePaths, false);
		}
		this->dynamicTutorialElement->SetZOrder(100);
		this->dynamicTutorialElement->SetTouchHandlers(this);
		this->dynamicTutorialElement->SetClickable(true);
		/*
		UiElement* exitBtn = (UiElement*)ObjectRegistry::GetObjectByName(TUTORIAL_EXIT_BTN);
		
		if (this->tutorials[index].imageNames.size() == 1) {
			exitBtn->SetSpriteTextureIndex(0);
		} else {
			exitBtn->SetSpriteTextureIndex(1);
		}
		*/
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

		if (!SINGLETONS->GetLevelManager()->GetCurrentLevelInProgress()) {
			MessageChunk messageChunk = ENGINE->GetMessageHub()->GetOneFreeMessage();
			messageChunk->SetMessageType(MESSAGE_TYPE_OPEN_POST_MENU);
			ENGINE->GetMessageHub()->SendMulticastMessage(messageChunk);
		}
		return;
	}
	ASSERT(textureIndex < this->tutorials[this->currentTutorialIndex].imageNames.size(), "nextTutorialImage() has been called when the tutorial is out of images to show. ");
	this->dynamicTutorialElement->SetSpriteTextureIndex(textureIndex);
	if(textureIndex + 1 == this->tutorials[this->currentTutorialIndex].imageNames.size()) {
		/*
		UiElement* exitBtn = (UiElement*)ObjectRegistry::GetObjectByName(TUTORIAL_EXIT_BTN);
		exitBtn->SetSpriteTextureIndex(0);
		*/

	} else if(textureIndex == this->tutorials[this->currentTutorialIndex].imageNames.size()) {
		UiObject* tut = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[TUTORIAL_MENU]);
		SINGLETONS->GetMenuManager()->TweenOutUiObject(tut);
	}
}

void GameUiTouchHandlers::onLevelEnd(bool success) {
	if (success && this->isTutorialLevel && !ENGINE->GetSceneGraph3D()->IsPaused()) {
		for (unsigned int i = 0; i < this->tutorials.size(); ++i) {
			if ((!this->tutorials[i].hasFired) && (this->tutorials[i].msgType == MESSAGE_TYPE_LEVEL_WON)) {
				return;
			}
		}
	}

	this->showPostMenu(success);
}

void GameUiTouchHandlers::showPostMenu(bool success) {
	ENGINE->GetSceneGraph3D()->Pause();
	UiObject* postMenu;
	if(success){
		 postMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[POST_GAME_WIN_MENU]);
		 SINGLETONS->GetMenuManager()->UpdateMenuWithMastery(POST_GAME_WIN_MENU, SINGLETONS->GetLevelManager()->GetCurrentLevelIndex());
		 SINGLETONS->GetMenuManager()->PlayBGM(LEVEL_WIN_BGM, false);

	} else {
		 postMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[POST_GAME_LOSE_MENU]);
		 SINGLETONS->GetMenuManager()->PlayBGM(LEVEL_LOSE_BGM, false);

	}
	postMenu->SetVisible(true);
	SINGLETONS->GetMenuManager()->TweenInUiObject(postMenu);
	//this->tutorials.clear();
}

void GameUiTouchHandlers::init() {
	this->tutorialCache_ref = nullptr;

	this->isTutorialLevel = false;
	this->dynamicTutorialElement = nullptr;
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_SELECTED_UNIT_CHANGED);
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_CREATED_TUTORIAL);
	//this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_ON_END_CONDITIONS_MET);
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_SCENE_START);
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_LEVEL_WON);
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_LEVEL_LOST);
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_OPEN_POST_MENU);
}

void GameUiTouchHandlers::destroy() {
	this->tutorialCache_ref = nullptr;

	if(this->dynamicTutorialElement != nullptr) {
		delete this->dynamicTutorialElement;
		this->dynamicTutorialElement = nullptr;
	}
}
