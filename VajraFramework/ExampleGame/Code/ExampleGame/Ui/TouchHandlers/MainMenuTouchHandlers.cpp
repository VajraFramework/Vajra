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
#include "Vajra/Framework/Settings/Settings.h"

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

#define BRIGHTNESS_SETTING "brightness_setting"
#define AMBIENTLIGHTING_SETTING "ambientlighting_setting"
#define SHADOWS_SETTING "shadows_setting"
#define TEXTURESAMPLING_SETTING "texturesampling_setting"

#define SPRITE_INDEX_off 0
#define SPRITE_INDEX_low 1
#define SPRITE_INDEX_medium 2
#define SPRITE_INDEX_high 3
#define SPRITE_INDEX_num_settings_sprites 4

MainMenuTouchHandlers::MainMenuTouchHandlers() {
	this->missionRoot = nullptr;
	this->currentScreenX = 0;
	this->currentMission = 0;
	missionStartX.push_back(-32.0f);
	missionStartX.push_back(-1160.0f);
	missionStartX.push_back(-2032.0f);
}

void MainMenuTouchHandlers::OnTouchDownHandlers(UiObject* uiObject, Touch /*touch*/) {
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
		for(int i = 0; i < (int)this->currentLevelButtons[this->currentMission].size(); i++) {
			if(this->currentLevelButtons[this->currentMission][i] == uiObject) {
				std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "gameUi.uiscene";
				int levelOffset = 0;
				for(int j = 0; j < this->currentMission; ++j) {
					levelOffset += SINGLETONS->GetLevelManager()->GetNumLevelsInMission(j);
				}
				SINGLETONS->GetMenuManager()->LoadLevel(i + levelOffset);
				return;
			}
		}
	}
	if (uiObject->GetName() == "play") {
		UiElement* preMenuBackground = (UiElement*)ObjectRegistry::GetObjectByName(START_MENU);
		preMenuBackground->SetVisible(false);
		this->missionRoot->SetVisible(true);	
		this->scrollToCurrentMission();
		
	} else if(uiObject->GetName() == "options") {
		UiObject* startMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[START_MENU]);
		UiObject* optionsMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[OPTIONS_MENU]);
		startMenu->SetVisible(false);
		optionsMenu->SetVisible(true);
		this->displaySettings();
		
	} else if (uiObject->GetName() == BRIGHTNESS_SETTING) {
		UiElement* uiElement = (UiElement*)ObjectRegistry::GetObjectByName(BRIGHTNESS_SETTING);
		if (uiElement != nullptr) {
			ASSERT(uiElement->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a UiElement");
			uiElement->SetSpriteTextureIndex((uiElement->GetSpriteTextureIndex() + 1) % SPRITE_INDEX_num_settings_sprites);
		}

	} else if (uiObject->GetName() == AMBIENTLIGHTING_SETTING) {
		UiElement* uiElement = (UiElement*)ObjectRegistry::GetObjectByName(AMBIENTLIGHTING_SETTING);
		if (uiElement != nullptr) {
			ASSERT(uiElement->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a UiElement");
			uiElement->SetSpriteTextureIndex((uiElement->GetSpriteTextureIndex() + 1) % SPRITE_INDEX_num_settings_sprites);
		}

	} else if (uiObject->GetName() == SHADOWS_SETTING) {
		UiElement* uiElement = (UiElement*)ObjectRegistry::GetObjectByName(SHADOWS_SETTING);
		if (uiElement != nullptr) {
			ASSERT(uiElement->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a UiElement");
			uiElement->SetSpriteTextureIndex((uiElement->GetSpriteTextureIndex() + 1) % SPRITE_INDEX_num_settings_sprites);
		}

	} else if (uiObject->GetName() == TEXTURESAMPLING_SETTING) {
		UiElement* uiElement = (UiElement*)ObjectRegistry::GetObjectByName(TEXTURESAMPLING_SETTING);
		if (uiElement != nullptr) {
			ASSERT(uiElement->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a UiElement");
			uiElement->SetSpriteTextureIndex((uiElement->GetSpriteTextureIndex() + 1) % SPRITE_INDEX_num_settings_sprites);
		}

	} else if(uiObject->GetName() == "back") {
		UiObject* startMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[START_MENU]);
		UiObject* optionsMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[OPTIONS_MENU]);
		startMenu->SetVisible(true);
		optionsMenu->SetVisible(false);
		
	} else if(uiObject->GetName() == "save_settings") {
		UiObject* startMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[START_MENU]);
		UiObject* optionsMenu = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(this->uiSceneObjects[OPTIONS_MENU]);
		startMenu->SetVisible(true);
		optionsMenu->SetVisible(false);
		this->applySettings();
	}
}

void MainMenuTouchHandlers::parallaxScroll(UiObject* parallaxRoot, float xDiff, bool touchEnd) {
	float frontTransAmt = 0;
	if(touchEnd) {
		UiObject* pScreen = (UiObject*)ObjectRegistry::GetObjectByName(PARALLAX_FRONT);
		float screenX = pScreen->GetTransform()->GetPositionWorld().x;
		float dist = this->missionStartX[this->currentMission] - screenX;
		
		if(screenX > this->missionStartX[this->currentMission]) { // LEFT
			if(this->currentMission > 0) {
				if(std::abs(dist) > std::abs(this->missionStartX[this->currentMission - 1] - screenX)) {
					this->currentMission--;
				}
			}
		} else if(screenX < this->missionStartX[this->currentMission]) { // RIGHT
			if(this->currentMission < (int)this->missionStartX.size() - 1) {
				if(std::abs(dist) > std::abs(this->missionStartX[this->currentMission + 1] - screenX)) {
					this->currentMission++;
				}
			}
		}
		for(int j = 0; j < SINGLETONS->GetLevelManager()->GetNumMissions(); j++) {
			bool isVisible = j == this->currentMission;
			for(int i = 0; i < SINGLETONS->GetLevelManager()->GetNumLevelsInMission(j); i++) {
				this->currentLevelButtons[j][i]->SetVisible(isVisible);
			}
		}
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
						frontTransAmt / 2048.0f,
						false,
						INTERPOLATION_TYPE_LINEAR);
			}
		}
	}
}

void MainMenuTouchHandlers::scrollToCurrentMission() {
	for(int j = 0; j < SINGLETONS->GetLevelManager()->GetNumMissions(); j++) {
		for(int i = 0; i < SINGLETONS->GetLevelManager()->GetNumLevelsInMission(j); i++) {
			this->currentLevelButtons[j][i]->SetVisible(false);
		}
	}
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

	for(int j = 0; j < SINGLETONS->GetLevelManager()->GetNumMissions(); j++) {
		float missionXOffset = this->missionStartX[j];
		std::vector<UiObject*> levelPips;
		this->currentLevelButtons.push_back(levelPips);
		for(int i = 0; i < SINGLETONS->GetLevelManager()->GetNumLevelsInMission(j); i++) {
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
			uiElement->SetPosition(levelData.pinX - missionXOffset, levelData.pinY);
			uiElement->SetZOrder(10);
			this->currentLevelButtons[j].push_back((UiObject*)uiElement);

			int screenIndex = levelData.parallaxScreen;
			if(screenIndex < 3) {
				parallaxScreens[screenIndex]->AddChild(uiElement->GetId());
			}
		}
	}
}



int getSpriteIndexForSettingLevel(SettingLevel_t settingLevel) {
	switch (settingLevel) {
	case SETTING_LEVEL_off: return SPRITE_INDEX_off;
	case SETTING_LEVEL_low: return SPRITE_INDEX_low;
	case SETTING_LEVEL_medium: return SPRITE_INDEX_medium;
	case SETTING_LEVEL_high: return SPRITE_INDEX_high;
	default: ASSERT(0, "Valid setting level, %d", settingLevel);
	}
	return SPRITE_INDEX_off;
}

SettingLevel_t getSettingLevelForSpriteIndex(int spriteIndex) {
	switch (spriteIndex) {
	case SPRITE_INDEX_off: return SETTING_LEVEL_off;
	case SPRITE_INDEX_low: return SETTING_LEVEL_low;
	case SPRITE_INDEX_medium: return SETTING_LEVEL_medium;
	case SPRITE_INDEX_high: return SETTING_LEVEL_high;
	default: ASSERT(0, "Valid sprite index, %d", spriteIndex);
	}
	return SETTING_LEVEL_medium;
}


void MainMenuTouchHandlers::displaySettings() {
	{
		UiElement* uiElement = (UiElement*)ObjectRegistry::GetObjectByName(BRIGHTNESS_SETTING);
		if (uiElement != nullptr) {
			ASSERT(uiElement->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a UiElement");
			uiElement->SetSpriteTextureIndex(getSpriteIndexForSettingLevel(FRAMEWORK->GetSettings()->GetSetting(SETTING_TYPE_brightness)));
		}
	}
	{
		UiElement* uiElement = (UiElement*)ObjectRegistry::GetObjectByName(AMBIENTLIGHTING_SETTING);
		if (uiElement != nullptr) {
			ASSERT(uiElement->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a UiElement");
			uiElement->SetSpriteTextureIndex(getSpriteIndexForSettingLevel(FRAMEWORK->GetSettings()->GetSetting(SETTING_TYPE_ambient_lighting)));
		}
	}
	{
		UiElement* uiElement = (UiElement*)ObjectRegistry::GetObjectByName(SHADOWS_SETTING);
		if (uiElement != nullptr) {
			ASSERT(uiElement->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a UiElement");
			uiElement->SetSpriteTextureIndex(getSpriteIndexForSettingLevel(FRAMEWORK->GetSettings()->GetSetting(SETTING_TYPE_shadows)));
		}
	}
	{
		UiElement* uiElement = (UiElement*)ObjectRegistry::GetObjectByName(TEXTURESAMPLING_SETTING);
		if (uiElement != nullptr) {
			ASSERT(uiElement->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a UiElement");
			uiElement->SetSpriteTextureIndex(getSpriteIndexForSettingLevel(FRAMEWORK->GetSettings()->GetSetting(SETTING_TYPE_texture_sampling)));
		}
	}
}

void MainMenuTouchHandlers::applySettings() {
	{
		UiElement* uiElement = (UiElement*)ObjectRegistry::GetObjectByName(BRIGHTNESS_SETTING);
		if (uiElement != nullptr) {
			ASSERT(uiElement->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a UiElement");
			FRAMEWORK->GetSettings()->SetSetting(SETTING_TYPE_brightness, getSettingLevelForSpriteIndex(uiElement->GetSpriteTextureIndex()));
		}
	}
	{
		UiElement* uiElement = (UiElement*)ObjectRegistry::GetObjectByName(AMBIENTLIGHTING_SETTING);
		if (uiElement != nullptr) {
			ASSERT(uiElement->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a UiElement");
			FRAMEWORK->GetSettings()->SetSetting(SETTING_TYPE_ambient_lighting, getSettingLevelForSpriteIndex(uiElement->GetSpriteTextureIndex()));
		}
	}
	{
		UiElement* uiElement = (UiElement*)ObjectRegistry::GetObjectByName(SHADOWS_SETTING);
		if (uiElement != nullptr) {
			ASSERT(uiElement->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a UiElement");
			FRAMEWORK->GetSettings()->SetSetting(SETTING_TYPE_shadows, getSettingLevelForSpriteIndex(uiElement->GetSpriteTextureIndex()));
		}
	}
	{
		UiElement* uiElement = (UiElement*)ObjectRegistry::GetObjectByName(TEXTURESAMPLING_SETTING);
		if (uiElement != nullptr) {
			ASSERT(uiElement->GetClassType() & CLASS_TYPE_UIELEMENT, "Object is a UiElement");
			FRAMEWORK->GetSettings()->SetSetting(SETTING_TYPE_texture_sampling, getSettingLevelForSpriteIndex(uiElement->GetSpriteTextureIndex()));
		}
	}
}
