#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "ExampleGame/Components/LevelManager/LevelLoader.h"
#include "ExampleGame/Messages/Declarations.h"
#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"
#include "Vajra/Common/Objects/ObjectRegistry.h"
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
#define TURORIAL_MENU "tutorialScreen"

#define TUTORIAL_EXIT_BTN "closeTutorial"
#define TUTORIAL_NEXT_BTN "nextTutorial"
#define DYNAMIC_TUTORIAL_ELEMENT "dynamicTutorial"
#define ASSASSIN_ICON_INDEX 0
#define THIEF_ICON_INDEX 1

void onTutorialTweenInComplete(ObjectIdType gameObjectId, std::string tweenClipName) {
UiObject* tutorialMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(gameObjectId);
	ASSERT(tutorialMenu != nullptr, "Tutorial menu object is null in onTutorialTweenInComplete");
	if(tutorialMenu != nullptr) {
		tutorialMenu->SetClickable(true);
	}

}
void onTutorialTweenOutComplete(ObjectIdType gameObjectId, std::string tweenClipName) {
	UiObject* tutorialMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(gameObjectId);
	ASSERT(tutorialMenu != nullptr, "Tutorial menu object is null in onTutorialTweenOutComplete");
	if(tutorialMenu != nullptr) {
		tutorialMenu->SetClickable(false);
	}
	
	Object* dynamicTutorial = ObjectRegistry::GetObjectByName(DYNAMIC_TUTORIAL_ELEMENT);
	if(dynamicTutorial != nullptr) {
		delete dynamicTutorial;
	}
}
GameUiTouchHandlers::GameUiTouchHandlers() : UiTouchHandlers() {
	this->isTutorialLevel = false;
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_SELECTED_UNIT_CHANGED);
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_CREATED_TUTORIAL);
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
				// TODO [Implement] Ensure type safety here
				UiElement* changeUnitIcon = (UiElement*)ObjectRegistry::GetObjectByName("changeUnit");
				changeUnitIcon->SetSpriteTextureIndex(ASSASSIN_ICON_INDEX);

			} else if(messageChunk->messageData.iv1.y == UNIT_TYPE_THIEF) {
				// TODO [Implement] Ensure type safety here
				UiElement* changeUnitIcon = (UiElement*)ObjectRegistry::GetObjectByName("changeUnit");
				changeUnitIcon->SetSpriteTextureIndex(THIEF_ICON_INDEX);
			}
			break;
		case MESSAGE_TYPE_CREATED_TUTORIAL:
			this->setupTutorial(messageChunk->messageData.s);
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
	if (uiObject->GetName() == "pause") {
		UiObject* pauseMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[PAUSE_MENU]);
		pauseMenu->SetVisible(!pauseMenu->IsVisible());
		ENGINE->GetMessageHub()->SendMulticastMessage(MESSAGE_TYPE_PAUSE);

	} else if (uiObject->GetName() == "resume") {
		UiObject* pauseMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[PAUSE_MENU]);
		pauseMenu->SetVisible(false);
		ENGINE->GetMessageHub()->SendMulticastMessage(MESSAGE_TYPE_UNPAUSE);
	} else if (uiObject->GetName() == "mission_from_pause") {
		UiObject* pauseMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[PAUSE_MENU]);
		pauseMenu->SetVisible(false);

		//ENGINE->GetSceneGraph3D()->UnloadCurrentScene();

		std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "mainMenu.uiscene";
		//
		UiSceneLoader::UnloadCurrentUiScene();
		UiSceneLoader::LoadUiSceneFromUiSceneFile(pathToTestUiScene.c_str(), new MainMenuTouchHandlers());

	} else if(uiObject->GetName() == "changeUnit") {
		SINGLETONS->GetGridManager()->SwitchSelectedUnit();

	} else if(uiObject->GetName() == TUTORIAL_EXIT_BTN) {
		UiObject* tut = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[TURORIAL_MENU]);
		tut->SetClickable(false);
		ENGINE->GetTween()->TweenPosition(tut->GetId(),
										  tut->GetTransform()->GetPosition(),
										  glm::vec3(tut->GetTransform()->GetPosition().x, -768.0f, tut->GetTransform()->GetPosition().z),
										  1.0f,
										  false,
										  TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
										  false,
										  onTutorialTweenOutComplete);
	} else if(uiObject->GetName() == TUTORIAL_NEXT_BTN) {
		this->nextTutorialImage();
	}
}



MessageType stringToMessageType(std::string msgString) {
	if(msgString == "MESSAGE_TYPE_GRID_ROOM_ENTERED") {
		return MESSAGE_TYPE_GRID_ROOM_ENTERED;
	} else if (msgString == "MESSAGE_TYPE_GRID_ZONE_ENTERED") {
		return MESSAGE_TYPE_GRID_ZONE_ENTERED;
	} else if(msgString == "MESSAGE_TYPE_SELECTED_UNIT_CHANGED") {
		return MESSAGE_TYPE_SELECTED_UNIT_CHANGED;
	}
	ASSERT(true, "stringToMessageType has reached the end without returning a message. Did you add a case for %s?", msgString.c_str());
	return 0;
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
		XmlNode* messageNode = tutorialNode->GetFirstChildByNodeName("message");
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
					cell = messageNode->GetFirstChildByNodeName("cell");
					tData.vector3Data = glm::vec3(cell->GetAttributeValueI("x"), 0.0f, cell->GetAttributeValueI("z"));
					break;
				case MESSAGE_TYPE_GRID_ROOM_ENTERED:
					cell = messageNode->GetFirstChildByNodeName("cell");
					tData.vector3Data = SINGLETONS->GetGridManager()->GetGrid()->GetRoomCenter(cell->GetAttributeValueI("x"),
									    cell->GetAttributeValueI("z"));
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
			
			messageNode = messageNode->GetNextSiblingByNodeName("message");
												 
		}
		//XmlNode* image = tutorialNode->GetFirstChildByNodeName("image");
		//ASSERT(image != nullptr, "Tutorial node does not have an image node");
		
	}
	delete parser;
}

void GameUiTouchHandlers::tryTutorial(int index, MessageChunk messageChunk) {
	switch(this->tutorials[index].msgType) {
		case MESSAGE_TYPE_GRID_ZONE_ENTERED:
			if(this->tutorials[index].vector3Data != messageChunk->messageData.fv1) {
				//return;
			}
			break;
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
	UiObject* tut = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[TURORIAL_MENU]);
	tut->SetClickable(false);
	

	// Load the textures for the tutorial
	this->dynamicTutorialElement = new UiElement(ENGINE->GetSceneGraphUi());
	this->dynamicTutorialElement->SetName(DYNAMIC_TUTORIAL_ELEMENT);
	if (this->tutorials[index].imageNames.size() != 0) {
		std::vector<std::string> imagePaths;
		for (std::string imageName : this->tutorials[index].imageNames) {
			imagePaths.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + imageName);
		}
		this->dynamicTutorialElement->InitSprite(200, 200, "sptshdr", imagePaths);

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
	//tut->SetZOrder(120);
	//tut->AddChild(this->dynamicTutorialElement->GetId()); //->SetParent(tut->GetId());
	
	// tween in the tutorial
	ENGINE->GetTween()->TweenPosition(tut->GetId(),
									  tut->GetTransform()->GetPosition(),
									  glm::vec3(tut->GetTransform()->GetPosition().x, -300.0f, tut->GetTransform()->GetPosition().z),
									  1.0f,
									  false,
									  TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
									  false,
									  onTutorialTweenInComplete);
}

void GameUiTouchHandlers::nextTutorialImage() {
	int textureIndex = this->dynamicTutorialElement->GetSpriteTextureIndex();
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

