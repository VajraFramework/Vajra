#include "ExampleGame/Ui/MenuManager/MenuDefinitions.h"
#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"

#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioSource.h"
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


#define BRIGHTNESS_SETTING "brightness_setting"
#define AMBIENTLIGHTING_SETTING "ambientlighting_setting"
#define SHADOWS_SETTING "shadows_setting"
#define TEXTURESAMPLING_SETTING "texturesampling_setting"

#define SPRITE_INDEX_off 0
#define SPRITE_INDEX_low 1
#define SPRITE_INDEX_medium 2
#define SPRITE_INDEX_high 3
#define SPRITE_INDEX_num_settings_sprites 4

#define PIP_AVAILABLE 0
#define PIP_COMPLETE_BONUS 1
#define PIP_COMPELTE_NO_BONUS 2
#define PIP_LOCKED 3
MainMenuTouchHandlers::MainMenuTouchHandlers() {
	this->missionRoot = nullptr;
	this->currentScreenX = 0;
	this->currentMissionScreenIndex = 0;
	this->prevContractIndex = -1;
	// Todo [Implement]  data drive this
	int missionStartXs[3] = { -32, -1160, -2032 };
	int dummy = 10;
	UiSceneLoader::AdjustPositionForResolution(missionStartXs[0], dummy, "LEFT", "TOP", dummy, dummy, 1024, 768);
	UiSceneLoader::AdjustPositionForResolution(missionStartXs[1], dummy, "LEFT", "TOP", dummy, dummy, 1024, 768);
	UiSceneLoader::AdjustPositionForResolution(missionStartXs[2], dummy, "LEFT", "TOP", dummy, dummy, 1024, 768);
	missionStartX.push_back(missionStartXs[0]);
	missionStartX.push_back(missionStartXs[1]);
	missionStartX.push_back(missionStartXs[2]);
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
	SINGLETONS->GetMenuManager()->PlaySFX(BUTTON_CLICK_SFX);

	if(uiObject->GetName() == "backMenuButton") {
		this->goBackOneMenu();
		return;
	}

	// Contract Selection
	ASSERT(this->contractRoot != nullptr, "The contractRoot root is not null");
	if(this->contractRoot != nullptr && this->contractRoot->IsVisible()) {
		if (uiObject->GetName() == "contractOne") {
			this->openMissionMenu(0);
		} else if (uiObject->GetName() == "contractTwo") {
			this->openMissionMenu(1);
		}
	}
	// Level Selection
	ASSERT(this->parallaxRoot != nullptr, "The parallaxRoot root is not null");
	if(this->parallaxRoot != nullptr && this->parallaxRoot->IsVisible()) {
		if(uiObject->GetName() == PARALLAX) {
			float xDiff = touch.pos.x - touch.prevPos.x;
			this->parallaxScroll(uiObject, xDiff, true);
		}
		for(int i = 0; i < (int)this->currentLevelButtons[this->currentMissionScreenIndex].size(); i++) {
			if(this->currentLevelButtons[this->currentMissionScreenIndex][i] == uiObject) {
				LevelData* levelData = SINGLETONS->GetLevelManager()->GetLevelData(this->currentMissionScreenIndex, i);
				if(levelData->completion != LevelCompletion::Locked) {
					std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "gameUi.uiscene";
					SINGLETONS->GetMenuManager()->LoadLevel(i);
					return;
				}
			}
		}
	}


	// Main Menu and Options
	if (uiObject->GetName() == "play") {
		this->openContractMenu();
		//this->scrollToCurrentMission();
		
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
		float dist = this->missionStartX[this->currentMissionScreenIndex] - screenX;
		
		if(screenX > this->missionStartX[this->currentMissionScreenIndex]) { // LEFT
			if(this->currentMissionScreenIndex > 0) {
				if(std::abs(dist) > std::abs(this->missionStartX[this->currentMissionScreenIndex - 1] - screenX)) {
					this->currentMissionScreenIndex--;
				}
			}
		} else if(screenX < this->missionStartX[this->currentMissionScreenIndex]) { // RIGHT
			if(this->currentMissionScreenIndex < (int)this->missionStartX.size() - 1) {
				if(std::abs(dist) > std::abs(this->missionStartX[this->currentMissionScreenIndex + 1] - screenX)) {
					this->currentMissionScreenIndex++;
				}
			}
		}
		SINGLETONS->GetLevelManager()->SetCurrentMission(currentMissionScreenIndex);
		for(int j = 0; j < SINGLETONS->GetLevelManager()->GetNumMissionsInCurrentContract(); j++) {
			bool isVisible = j == this->currentMissionScreenIndex;
			for(int i = 0; i < SINGLETONS->GetLevelManager()->GetNumLevelsInMission(j); i++) {
				this->currentLevelButtons[j][i]->SetVisible(isVisible);
			}
		}
		frontTransAmt = this->missionStartX[this->currentMissionScreenIndex] - pScreen->GetTransform()->GetPositionWorld().x; 
	}
	for(ObjectIdType id : parallaxRoot->GetChildren()){
		
		UiObject* object = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(id);
		glm::vec3 pos = object->GetTransform()->GetPositionWorld();
		float moveAmt;
		if(!touchEnd) {
			moveAmt = xDiff;
		} else {
			moveAmt = this->missionStartX[this->currentMissionScreenIndex];
		}

		if(object->GetName() == PARALLAX_FRONT) {
			object->SetZOrder(3);

		} else if(object->GetName() == PARALLAX_MIDDLE) {
			object->SetZOrder(2);
			moveAmt *= .5f;
		} else if(object->GetName() == PARALLAX_BACK) {
			object->SetZOrder(1);
			moveAmt *= .25f;
		} else {
			object->SetZOrder(50);
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
	int curMissionIndex = SINGLETONS->GetLevelManager()->GetCurrentMission();
	for(int j = 0; j < SINGLETONS->GetLevelManager()->GetNumMissionsInCurrentContract(); j++) {
		for(int i = 0; i < SINGLETONS->GetLevelManager()->GetNumLevelsInMission(j); i++) {
			this->currentLevelButtons[j][i]->SetVisible(j == curMissionIndex);
		}
	}
	for(ObjectIdType id : parallaxRoot->GetChildren()){
	
		UiObject* object = (UiObject*)ENGINE->GetSceneGraphUi()->GetGameObjectById(id);
		glm::vec3 pos = object->GetTransform()->GetPositionWorld();
		float newX = missionStartX[curMissionIndex];
		if(object->GetName() == PARALLAX_FRONT) {

		} else if(object->GetName() == PARALLAX_MIDDLE) {
			newX *= .5f;
		} else if(object->GetName() == PARALLAX_BACK) {
			newX *= .25f;
		} else {
			continue;
		}
		pos.x = newX;
		object->GetTransform()->SetPosition(pos);
	}
	
}
void MainMenuTouchHandlers::createMissionMenu() {
	this->missionRoot = (UiElement*)ObjectRegistry::GetObjectByName("missionMenu");
	this->contractRoot = (UiElement*)ObjectRegistry::GetObjectByName(CONTRACT);
	this->parallaxRoot = (UiElement*)ObjectRegistry::GetObjectByName(PARALLAX);
	
	UiElement* pScreen = (UiElement*)ObjectRegistry::GetObjectByName(PARALLAX_FRONT);
	ASSERT(pScreen != nullptr, "parallaxFront is not null");
	this->parallaxScreens.push_back(pScreen);
	pScreen = (UiElement*)ObjectRegistry::GetObjectByName(PARALLAX_MIDDLE);
	ASSERT(pScreen != nullptr, "parallaxMiddle is not null");
	this->parallaxScreens.push_back(pScreen);
	pScreen = (UiElement*)ObjectRegistry::GetObjectByName(PARALLAX_BACK);
	ASSERT(pScreen != nullptr, "parallaxBack is not null");
	this->parallaxScreens.push_back(pScreen);

	// load all pips
	for(int i = 0; i < MAX_LEVELS_PER_CONTRACT; i++) {
		UiElement* uiElement = new UiElement(ENGINE->GetSceneGraphUi());
		this->parallaxRoot->AddChild(uiElement->GetId());
		std::vector<std::string> imagePaths;
		imagePaths.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_LevelPIP_Available_01.png");
		imagePaths.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_LevelPIP_Bonus_01.png");
		imagePaths.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_LevelPIP_Completed_01.png");
		imagePaths.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_LevelPIP_Locked_01.png"); // TEMP : replace with locked image
		int width_out = 67;
		int height_out = 118;
		int dummy = 10;
		// TODO [Hack] 1024x768
		UiSceneLoader::AdjustPositionForResolution(dummy, dummy, "LEFT", "TOP", width_out, height_out, 1024, 768);
		uiElement->InitSprite(width_out, height_out, "ustshdr", imagePaths, true);
		uiElement->SetTouchHandlers(this);
		uiElement->SetClickable(true);
		uiElement->SetVisible(false);
		uiElement->SetZOrder(10);
		this->levelPips.push_back(uiElement);
	}
}

void MainMenuTouchHandlers::loadPips(int contractIndex) {
	for(int i = 0; i < 3; ++i) {
		this->parallaxScreens[i]->SetSpriteTextureIndex(contractIndex);
	}

	// hide all pips
	for(int i = 0; i < MAX_LEVELS_PER_CONTRACT; ++i) {
		this->parallaxRoot->AddChild(this->levelPips[i]->GetId());
		this->levelPips[i]->SetVisible(false);
	}

	// place the pips on the parallax screen
	int currentPipIndex = 0;
	for(int j = 0; j < SINGLETONS->GetLevelManager()->GetNumMissionsInCurrentContract(); j++) {
		int missionXOffset = this->missionStartX[j];
		std::vector<UiObject*> pips;
		this->currentLevelButtons.push_back(pips);
		for(int i = 0; i < SINGLETONS->GetLevelManager()->GetNumLevelsInMission(j); i++) {
			ASSERT(currentPipIndex < MAX_LEVELS_PER_CONTRACT, "the number of levels in this contract is less than the max");
			UiElement* uiElement = this->levelPips[currentPipIndex];
			LevelData* levelData = SINGLETONS->GetLevelManager()->GetLevelData(j, i);
			uiElement->SetVisible(j == this->currentMissionScreenIndex);
			int dummy = 10;
			int posX = levelData->pinX;
			int posY = levelData->pinY;
			// TODO [Hack] 1024x768
			UiSceneLoader::AdjustPositionForResolution(posX, posY, "LEFT", "TOP", dummy, dummy, 1024, 768);
			posX = posX - missionXOffset;
			uiElement->SetPosition(posX, posY);
			uiElement->SetZOrder(10);
			switch(levelData->completion) {
				case LevelCompletion::Locked:
					uiElement->SetSpriteTextureIndex(PIP_LOCKED);
					break;
				case LevelCompletion::Unlocked:
					uiElement->SetSpriteTextureIndex(PIP_AVAILABLE);
					break;
				case LevelCompletion::Completed:
					uiElement->SetSpriteTextureIndex(PIP_COMPELTE_NO_BONUS);
					break;
				default:
					uiElement->SetSpriteTextureIndex(PIP_COMPLETE_BONUS);
					break;
			}
			this->currentLevelButtons[j].push_back((UiObject*)uiElement);

			int screenIndex = levelData->parallaxScreen;
			if(screenIndex < 3) {
				this->parallaxScreens[screenIndex]->AddChild(uiElement->GetId());
			}

			currentPipIndex++;
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

void MainMenuTouchHandlers::openStartMenu() {
	UiElement* preMenuBackground = (UiElement*)ObjectRegistry::GetObjectByName(START_MENU);
	preMenuBackground->SetVisible(true);
	this->missionRoot->SetVisible(false);
}
void MainMenuTouchHandlers::openContractMenu() {
	UiElement* preMenuBackground = (UiElement*)ObjectRegistry::GetObjectByName(START_MENU);
	preMenuBackground->SetVisible(false);
	this->missionRoot->SetVisible(true);
	this->parallaxRoot->SetVisible(false);

}
void MainMenuTouchHandlers::openMissionMenu(int contractIndex) {
	UiElement* preMenuBackground = (UiElement*)ObjectRegistry::GetObjectByName(START_MENU);
	preMenuBackground->SetVisible(false);
	this->missionRoot->SetVisible(true);
	this->contractRoot->SetVisible(false);
	for(int i = 0; i < MAX_LEVELS_PER_CONTRACT; i++) {
		this->levelPips[i]->SetVisible(false);
	}

	if(SINGLETONS->GetLevelManager()->GetCurrentContract() != contractIndex) {
		SINGLETONS->GetLevelManager()->SetCurrentMission(0);
	}
	
	ASSERT(contractIndex < SINGLETONS->GetLevelManager()->NumContracts(), "contract index is less than number of contracts");
	if(contractIndex != this->prevContractIndex) {
		// TODO [Implement] when we have a user profile set this equal to the right value
		SINGLETONS->GetLevelManager()->SetCurrentContract(contractIndex);
		this->currentMissionScreenIndex = SINGLETONS->GetLevelManager()->GetCurrentMission();
		this->loadPips(contractIndex);
		this->prevContractIndex = contractIndex;
	}
	this->scrollToCurrentMission();
}

void MainMenuTouchHandlers::goBackOneMenu() {
	if(this->contractRoot->IsVisible()) {
		this->openStartMenu();
	} else {
		this->openContractMenu();
	}
}

