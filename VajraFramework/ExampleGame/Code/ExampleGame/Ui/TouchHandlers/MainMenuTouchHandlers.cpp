#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"

#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
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
}

void MainMenuTouchHandlers::OnTouchDownHandlers(UiObject* uiObject, Touch /* touch */) {
	if (uiObject->GetName() == "play") {
		// Do something

	} else {
		// Do something

	}

}

void MainMenuTouchHandlers::OnTouchMoveHandlers(UiObject* uiObject, Touch touch) {
	if(uiObject->GetName() == PARALLAX) {
		float xDiff = touch.pos.x - touch.prevPos.x; 
		this->parallaxScroll(uiObject, xDiff);
	}
}

void MainMenuTouchHandlers::OnTouchUpHandlers(UiObject* uiObject, Touch /* touch */) {
	ASSERT(this->missionRoot != nullptr, "The mission root is not null");
	if(this->missionRoot != nullptr && this->missionRoot->IsVisible()) {
		return;
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

void MainMenuTouchHandlers::parallaxScroll(UiObject* parallaxRoot, float xDiff) {
	for(ObjectIdType id : parallaxRoot->GetChildren()){
		UiObject* object = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(id);
		float moveAmt = 0;
		if(object->GetName() == PARALLAX_FRONT) {
			moveAmt = xDiff;
		} else if(object->GetName() == PARALLAX_MIDDLE) {
			moveAmt = xDiff * .5f;
		} else if(object->GetName() == PARALLAX_BACK) {
			moveAmt = xDiff * .1f;
		}
		object->GetTransform()->Translate(moveAmt, XAXIS);
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
	
	/*for(int i = 0; i < numPerRow; ++i) {
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
	}*/
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
