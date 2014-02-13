#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "ExampleGame/Messages/Declarations.h"
#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Ui/UiCallbackComponent/UiCallbackComponent.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"


#if DEBUG
#include "ExampleGame/Ui/TouchHandlers/DebugMenuTouchHandlers.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#endif

// Todo [HACK] when level loading is better we probably won't need all these
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"

#define IN_GAME_MENU "inGame"
#define PAUSE_MENU "pauseMenu"
#define POST_GAME_MENU "postGame"

#define ASSASSIN_ICON_PATH "SD_GUI_Dagger_02.png"
#define THIEF_ICON_PATH "SD_GUI_Cloak_02.png"

GameUiTouchHandlers::GameUiTouchHandlers() : UiTouchHandlers() {
	this->eventForwarder->GetComponent<UiCallbackComponent>()->SubscribeToMessage(MESSAGE_TYPE_SELECTED_UNIT_CHANGED);
}

void GameUiTouchHandlers::HandleMessageCallback(MessageChunk messageChunk) {
	switch(messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_SELECTED_UNIT_CHANGED:
			if(messageChunk->messageData.iv1.y == UNIT_TYPE_ASSASSIN) {
				printf("ASSASSIN!");
			} else if(messageChunk->messageData.iv1.y == UNIT_TYPE_THIEF) {
				printf("THIEF!");
			}
		default:
			break;
	}
}

void GameUiTouchHandlers::OnTouchDownHandlers(UiObject* uiObject, Touch /* touch */) {
	if (uiObject->GetUiObjectName() == "play") {
		// Do something

	} else {
		// Do something

	}

}

void GameUiTouchHandlers::OnTouchMoveHandlers(UiObject* uiObject, Touch /* touch */) {

	if (uiObject->GetUiObjectName() == "play") {
		// Do something

	} else {
		// Do something

	}

}

void GameUiTouchHandlers::OnTouchUpHandlers(UiObject* uiObject, Touch /* touch */) {
#ifdef DEBUG 
	if (uiObject->GetUiObjectName() == "debugMenu") {
		std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "debugMenu.uiscene";
		//
		UiSceneLoader::UnloadCurrentUiScene();
		UiSceneLoader::LoadUiSceneFromUiSceneFile(pathToTestUiScene.c_str(), new DebugMenuTouchHandlers());
		// Return immediately here because the current scene has been unloaded
		return;
	}
#endif
	if (uiObject->GetUiObjectName() == "pause") {
		UiObject* pauseMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[PAUSE_MENU]);
		pauseMenu->SetVisible(!pauseMenu->IsVisible());
	} else if (uiObject->GetUiObjectName() == "resume") {
		UiObject* pauseMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[PAUSE_MENU]);
		pauseMenu->SetVisible(false);
	} else if (uiObject->GetUiObjectName() == "mission") {
		UiObject* pauseMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[PAUSE_MENU]);
		pauseMenu->SetVisible(false);

		//ENGINE->GetSceneGraph3D()->UnloadCurrentScene();

		std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "mainMenu.uiscene";
		//
		UiSceneLoader::UnloadCurrentUiScene();
		UiSceneLoader::LoadUiSceneFromUiSceneFile(pathToTestUiScene.c_str(), new MainMenuTouchHandlers());

	} else if(uiObject->GetUiObjectName() == "changeUnit") {
		SINGLETONS->GetGridManager()->SwitchSelectedUnit();

	} 
}
