#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

// Todo [HACK] when level loading is better we probably won't need all these
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"

#define IN_GAME_MENU "inGame"
#define PAUSE_MENU "pauseMenu"
#define POST_GAME_MENU "postGame"

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
		UiSceneLoader::LoadUiSceneFromUiSceneFile(pathToTestUiScene.c_str(), new MainMenuTouchHandlers());
	} 
}
