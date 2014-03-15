#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"

#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Tween/Tween.h"
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
#define PARALLAX "parallax"
#define PARALLAX_FRONT "parallaxFront"
#define PARALLAX_MIDDLE "parallaxMid"
#define PARALLAX_BACK "parallaxBack"

MainMenuTouchHandlers::MainMenuTouchHandlers() {
	this->missionRoot = nullptr;
	this->currentScreenX = 0;
	this->currentMission = 0;
	missionStartX.push_back(-32.0f);
	missionStartX.push_back(-1160.0f);
	missionStartX.push_back(-2032.0f);
}

void MainMenuTouchHandlers::OnTouchDownHandlers(UiObject* uiObject, Touch touch) {
	if(uiObject->GetName() == PARALLAX) {
		for(ObjectIdType id : uiObject->GetChildren()){
			ENGINE->GetTween()->CancelPostitionTween(id);
		}
	}

}

void MainMenuTouchHandlers::OnTouchMoveHandlers(UiObject* uiObject, Touch touch) {
	if(uiObject->GetName() == PARALLAX) {
		float xDiff = touch.pos.x - touch.prevPos.x;
		this->parallaxScroll(uiObject, xDiff);
	}
}

void MainMenuTouchHandlers::OnTouchUpHandlers(UiObject* uiObject, Touch touch) {
	ASSERT(this->missionRoot != nullptr, "The mission root is not null");
	if(this->missionRoot != nullptr && this->missionRoot->IsVisible()) {
		if(uiObject->GetName() == PARALLAX) {
			float xDiff = touch.pos.x - touch.prevPos.x;
			this->parallaxScroll(uiObject, xDiff, true);
		}
		for(int i = 0; i < this->currentLevelButtons.size(); i++) {
			if(this->currentLevelButtons[i] == uiObject) {
				std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "gameUi.uiscene";
				SINGLETONS->GetMenuManager()->LoadLevel(i);
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
		
	}
}

void MainMenuTouchHandlers::parallaxScroll(UiObject* parallaxRoot, float xDiff, bool touchEnd) {
	float frontTransAmt = 0;
	if(touchEnd) {
		UiObject* pScreen = (UiObject*)ObjectRegistry::GetObjectByName(PARALLAX_FRONT);
		frontTransAmt = this->missionStartX[this->currentMission] - pScreen->GetTransform()->GetPositionWorld().x; 
	}
	for(ObjectIdType id : parallaxRoot->GetChildren()){
		
		UiObject* object = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(id);
		glm::vec3 pos = object->GetTransform()->GetPositionWorld();
		float moveAmt;
		if(!touchEnd) {
			moveAmt = xDiff;
		} else {
			moveAmt = this->missionStartX[this->currentMission];
		}

		if(object->GetName() == PARALLAX_FRONT) {

		} else if(object->GetName() == PARALLAX_MIDDLE) {
			moveAmt *= .5f;
		} else if(object->GetName() == PARALLAX_BACK) {
			moveAmt *= .25f;
		} else {
			continue;
		}
		if(!touchEnd) {
			if(pos.x + moveAmt > 0) {
				moveAmt = -pos.x;
			} else if(pos.x + moveAmt < 1024.0f - object->GetWidth()) {
				float maxX = 1024.0f - object->GetWidth();
				moveAmt = maxX - pos.x;
			}
			object->GetTransform()->Translate(moveAmt, XAXIS);
		} else {
			frontTransAmt = std::abs(frontTransAmt);
			if(frontTransAmt > 0) {
				ENGINE->GetTween()->TweenPosition(
						object->GetId(),
						pos,
						glm::vec3(moveAmt, pos.y, pos.z),
						frontTransAmt / 1024.0f,
						false,
						INTERPOLATION_TYPE_LINEAR);
			}
		}
	}
}

void MainMenuTouchHandlers::scrollToCurrentMission() {
	UiObject* parallaxRoot = (UiObject*)ObjectRegistry::GetObjectByName(PARALLAX);
	this->parallaxScroll(parallaxRoot, 0, true);
}
void MainMenuTouchHandlers::createMissionMenu() {
	this->missionRoot = (UiElement*)ObjectRegistry::GetObjectByName("missionMenu");
	
	std::vector<UiObject*> parallaxScreens;
	UiObject* pScreen = (UiObject*)ObjectRegistry::GetObjectByName(PARALLAX_FRONT);
	ASSERT(pScreen != nullptr, "parallaxFront is not null");
	parallaxScreens.push_back(pScreen);
	pScreen = (UiObject*)ObjectRegistry::GetObjectByName(PARALLAX_MIDDLE);
	ASSERT(pScreen != nullptr, "parallaxMiddle is not null");
	parallaxScreens.push_back(pScreen);
	pScreen = (UiObject*)ObjectRegistry::GetObjectByName(PARALLAX_BACK);
	ASSERT(pScreen != nullptr, "parallaxBack is not null");
	parallaxScreens.push_back(pScreen);

	float missionXOffset = this->missionStartX[this->currentMission];
	for(int i = 0; i < SINGLETONS->GetLevelManager()->NumLevels(); i++) {
		UiElement* uiElement = new UiElement(ENGINE->GetSceneGraphUi());
		ENGINE->GetSceneGraphUi()->GetRootGameObject()->AddChild(uiElement->GetId());
		this->missionRoot->AddChild(uiElement->GetId());
		std::vector<std::string> imagePaths;
		imagePaths.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_LevelSelection_Level1_Marker.png");
		uiElement->InitSprite(67, 118, "ustshdr", imagePaths, true);
		uiElement->SetTouchHandlers(this);
		uiElement->SetClickable(true);
		uiElement->SetVisible(false);

		LevelData levelData = SINGLETONS->GetLevelManager()->GetLevelData(i);
		uiElement->SetPosition(levelData.pinX + missionXOffset, levelData.pinY);
		uiElement->SetZOrder(10);
		this->currentLevelButtons.push_back((UiObject*)uiElement);

		int screenIndex = levelData.parallaxScreen;
		if(screenIndex < 3) {
			parallaxScreens[screenIndex]->AddChild(uiElement->GetId());
		}
	}

	this->scrollToCurrentMission();
	/*for(int i = 0; i < numPerRow; ++i) {
		for(int j = 0; j < numPerRow; ++j) {
			int levelNum = j + i * numPerRow;
			if(levelNum >= SINGLETONS->GetLevelManager()->NumLevels()) {
				break;
			}
			UiElement* uiElement = new UiElement(ENGINE->GetSceneGraphUi());
			ENGINE->GetSceneGraphUi()->GetRootGameObject()->AddChild(uiElement->GetId());
			this->missionRoot->AddChild(uiElement->GetId());
			std::vector<std::string> imagePaths;
			imagePaths.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_LevelSelection_Level1_Marker.png");
			uiElement->InitSprite(67, 118, "ustshdr", imagePaths, false);
			uiElement->SetTouchHandlers(this);
			uiElement->SetClickable(true);
			uiElement->SetVisible(false);
			uiElement->SetPosition(j * buttonWidth + margin * (1+j), i * buttonHeight + margin * (1+i));
			uiElement->SetZOrder(10);
		}
	}*/
}
