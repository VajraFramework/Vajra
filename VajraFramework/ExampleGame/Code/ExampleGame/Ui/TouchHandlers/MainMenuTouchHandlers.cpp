#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"

#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
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

MainMenuTouchHandlers::MainMenuTouchHandlers() {
	this->missionRoot = nullptr;
}

void MainMenuTouchHandlers::OnTouchDownHandlers(UiObject* uiObject, Touch /* touch */) {
	if (uiObject->GetName() == "play") {
		// Do something

	} else {
		// Do something

	}

}

void MainMenuTouchHandlers::OnTouchMoveHandlers(UiObject* uiObject, Touch /* touch */) {
	if (uiObject->GetName() == "play") {
		// Do something

	} else {
		// Do something

	}

}

void MainMenuTouchHandlers::OnTouchUpHandlers(UiObject* uiObject, Touch /* touch */) {
	ASSERT(this->missionRoot != nullptr, "The mission root is not null");
	if(this->missionRoot != nullptr && this->missionRoot->IsVisible()) {
		int levelToLoad = -1;
		for(ObjectIdType id : this->missionRoot->GetChildren()){
			if(uiObject->GetId() != id) {
				GameObject* child = ENGINE->GetSceneGraphUi()->GetGameObjectById(id);
				if(child->GetClassType() & CLASS_TYPE_UIELEMENT) { // We only care about UIELEMENT children
					levelToLoad++;
				}
			} else {
				std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "gameUi.uiscene";
				SINGLETONS->GetMenuManager()->LoadLevel(levelToLoad);
				return;
			}
		}
	}
	if (uiObject->GetName() == "play") {
		UiElement* preMenuBackground = (UiElement*)ObjectRegistry::GetObjectByName(START_MENU);
		preMenuBackground->SetVisible(false);
		this->missionRoot->SetVisible(true);
	} else if(uiObject->GetName() == "options") {
		UiObject* startMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[START_MENU]);
		UiObject* optionsMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[OPTIONS_MENU]);
		startMenu->SetVisible(false);
		optionsMenu->SetVisible(true);
		
	} else if(uiObject->GetName() == "back") {
		UiObject* startMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[START_MENU]);
		UiObject* optionsMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[OPTIONS_MENU]);
		startMenu->SetVisible(true);
		optionsMenu->SetVisible(false);
		
	}else {
		// Do something

	}
}

void MainMenuTouchHandlers::createMissionMenu() {
	this->missionRoot = (UiElement*)ObjectRegistry::GetObjectByName("missionMenu");
	float margin = 32.0f;
	float buttonZoneWidth = ((float)FRAMEWORK->GetDeviceProperties()->GetWidthPixels());
	float buttonZoneHeight = ((float)FRAMEWORK->GetDeviceProperties()->GetHeightPixels());
    int numPerRow = (int)ceil(sqrt(SINGLETONS->GetLevelManager()->NumLevels()));
	float buttonWidth = (buttonZoneWidth - margin * (numPerRow + 1)) / numPerRow;	
	float buttonHeight = (buttonZoneHeight - margin * (numPerRow + 1)) / numPerRow;
	
	for(int i = 0; i < numPerRow; ++i) {
		for(int j = 0; j < numPerRow; ++j) {
			int levelNum = j + i * numPerRow;
			if(levelNum >= SINGLETONS->GetLevelManager()->NumLevels()) {
				break;
			}
			UiElement* uiElement = new UiElement(ENGINE->GetSceneGraphUi());
			ENGINE->GetSceneGraphUi()->GetRootGameObject()->AddChild(uiElement->GetId());
			this->missionRoot->AddChild(uiElement->GetId());
			uiElement->InitSprite(buttonWidth, buttonHeight, "uscshdr", glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
			uiElement->SetTouchHandlers(this);
			uiElement->SetClickable(true);
			uiElement->SetVisible(false);
			uiElement->SetPosition(j * buttonWidth + margin * (1+j), i * buttonHeight + margin * (1+i));
			uiElement->SetZOrder(2);
		}
	}
}
