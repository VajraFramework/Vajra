#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/TestUiSceneTouchHandlers.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUI.h"
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


void MainMenuTouchHandlers::OnTouchDownHandlers(UiObject* uiObject, Touch touch) {
	if (uiObject->GetUiObjectName() == "play") {
		// Do something

	} else {
		// Do something

	}

}

void MainMenuTouchHandlers::OnTouchMoveHandlers(UiObject* uiObject, Touch touch) {
	FRAMEWORK->GetLogger()->dbglog("\nGot touch move: name (%s): %f, %f", uiObject->GetUiObjectName().c_str(), touch.pos.x, touch.pos.y);

	if (uiObject->GetUiObjectName() == "play") {
		// Do something

	} else {
		// Do something

	}

}

void MainMenuTouchHandlers::OnTouchUpHandlers(UiObject* uiObject, Touch touch) {
	if (uiObject->GetUiObjectName() == "play") {
		// Do something
		SINGLETONS->GetLevelManager()->LoadLevelFromFile(FRAMEWORK->GetFileSystemUtils()->GetDeviceBaseResourcesPath() + "levels/ExampleLevel.lvl");
		GameObject* testZone = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(testZone->GetId());
		GridZone* zone = testZone->AddComponent<GridZone>();
		zone->SetZoneBounds(3, 0, 5, 5);


		std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "testUiScene.uiscene";
		UiSceneLoader::LoadUiSceneFromUiSceneFile(pathToTestUiScene.c_str(), new TestUiSceneTouchHandlers());

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
