#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
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


#define START_MENU "startMenu"
#define OPTIONS_MENU "optionsMenu"
#define MISSION_MENU "missionMenu"

void MainMenuTouchHandlers::OnTouchDownHandlers(UiObject* uiObject, Touch /* touch */) {
	if (uiObject->GetUiObjectName() == "play") {
		// Do something

	} else {
		// Do something

	}

}

void MainMenuTouchHandlers::OnTouchMoveHandlers(UiObject* uiObject, Touch /* touch */) {
	if (uiObject->GetUiObjectName() == "play") {
		// Do something

	} else {
		// Do something

	}

}

void MainMenuTouchHandlers::OnTouchUpHandlers(UiObject* uiObject, Touch /* touch */) {
	if (uiObject->GetUiObjectName() == "play") {
		// Load the test level
		SINGLETONS->GetLevelManager()->LoadLevelFromFile(FRAMEWORK->GetFileSystemUtils()->GetDeviceBaseResourcesPath() + "levels/SD_Playtest2_03.lvl");
		GameObject* testZone = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(testZone->GetId());
		GridZone* zone = testZone->AddComponent<GridZone>();
		zone->SetZoneBounds(3, 0, 5, 5);


		std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "gameUi.uiscene";
		UiSceneLoader::LoadUiSceneFromUiSceneFile(pathToTestUiScene.c_str(), new GameUiTouchHandlers());

	} else if(uiObject->GetUiObjectName() == "options") {
		UiObject* startMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[START_MENU]);
		UiObject* optionsMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[OPTIONS_MENU]);
		startMenu->SetVisible(false);
		optionsMenu->SetVisible(true);
		
	} else if(uiObject->GetUiObjectName() == "back") {
		UiObject* startMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[START_MENU]);
		UiObject* optionsMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[OPTIONS_MENU]);
		startMenu->SetVisible(true);
		optionsMenu->SetVisible(false);
		
	}else {
		// Do something

	}

}
