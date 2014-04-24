#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/GameConstants/GameConstants.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "ExampleGame/Ui/MenuManager/MenuDefinitions.h"
#include "ExampleGame/Ui/MenuManager/MenuManager.h"
#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"

#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1I1F.h"
#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioSource.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Utilities/StringUtilities.h"

ComponentIdType MenuManager::componentTypeId = COMPONENT_TYPE_ID_LEVEL_MANAGER;

#define MIN_LOAD_TIME 0.3f
#define BACKDROP_MAX_ALPHA 0.7f

#define BACKDROP "popUpBack"
void menuManagerNumberTweenCallback(float /* fromNumber */, float toNumber , float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams) {
	if(tweenClipName == "extraLoadTime") {
		SINGLETONS->GetMenuManager()->hideLoadScreen();
	} else if(tweenClipName == "backDropFade") {
		glm::vec4 backdropColor = SINGLETONS->GetMenuManager()->backdrop->GetSpriteColor();
		backdropColor.a = currentNumber;
		SINGLETONS->GetMenuManager()->backdrop->SetSpriteColor(backdropColor);
		if(currentNumber == 0 && toNumber == 0) {
			SINGLETONS->GetMenuManager()->backdrop->SetVisible(false);
		}
	} else if (tweenClipName == "loadScreenFadeIn") {
		glm::vec4 loadScreenColor = SINGLETONS->GetMenuManager()->loadScreen->GetSpriteColor();
		loadScreenColor.a = currentNumber;
		SINGLETONS->GetMenuManager()->loadScreen->SetSpriteColor(loadScreenColor);
		if(currentNumber == toNumber && userParams != nullptr) {
			ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, 0.2f, INTERPOLATION_TYPE_LINEAR, true, false, false, "updatePreMenu", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, userParams, menuManagerNumberTweenCallback);
			ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, 0.4f, INTERPOLATION_TYPE_LINEAR, true, false, false, "levelLoadDelay", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, userParams, menuManagerNumberTweenCallback);
		}
	} else if (tweenClipName == "loadScreenFadeOut") {
		printf("\n current number : %f ", currentNumber);
		glm::vec4 loadScreenColor = SINGLETONS->GetMenuManager()->loadScreen->GetSpriteColor();
		loadScreenColor.a = currentNumber;
		SINGLETONS->GetMenuManager()->loadScreen->SetSpriteColor(loadScreenColor);
		if(currentNumber == toNumber) {
			SINGLETONS->GetMenuManager()->loadScreen->SetVisible(false);
		}
	} else if (tweenClipName == "updatePreMenu") {
		SINGLETONS->GetMenuManager()->updatePreMenu(userParams->i);
	} else if (tweenClipName == "levelLoadDelay") {
		SINGLETONS->GetMenuManager()->loadLevel_internal(userParams->i);
	}
	/* else if (tweenClipName == "backDropFade") {
		glm::vec4 backdropColor = SINGLETONS->GetMenuManager()->loadScreen->GetSpriteColor();
		backdropColor.a = currentNumber;
		SINGLETONS->GetMenuManager()->loadScreen->SetSpriteColor(backdropColor);
		if(currentNumber == 0 && toNumber == 0) {
			SINGLETONS->GetMenuManager()->loadScreen->SetVisible(false);
		}
	} */

}
MenuManager::MenuManager() : Component() {
	this->init();
}

MenuManager::MenuManager(Object* object_) : Component(object_) {
	this->init();
}

MenuManager::~MenuManager() {
	this->destroy();
}

void MenuManager::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_LEVEL_LOADED: {
			if(this->loadScreen != nullptr && this->loadScreen->IsVisible()) {
				float loadTime = ((float)ENGINE->GetTimer()->GetHighResAbsoluteTime()) - this->loadStartTime;
				ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, .5f, INTERPOLATION_TYPE_LINEAR, false, false, false, "extraLoadTime", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, NULL, menuManagerNumberTweenCallback);
				/*if(loadTime < GetFloatGameConstant(GAME_CONSTANT_min_load_time)) {
					float extraLoadTime = GetFloatGameConstant(GAME_CONSTANT_min_load_time) - loadTime;
					ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, extraLoadTime, INTERPOLATION_TYPE_LINEAR, false, false, false, "extraLoadTime", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, NULL, menuManagerNumberTweenCallback);
				} else {
					ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, .2f, INTERPOLATION_TYPE_LINEAR, false, false, false, "extraLoadTime", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, NULL, menuManagerNumberTweenCallback);
				}*/
			}
			break;
		}
		default:
			break;
	}

}

void MenuManager::init() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_LEVEL_LOADED, this->GetTypeId(), false);
	this->LoadMainMenu(START_MENU);
}

void MenuManager::destroy() {
	
}

void MenuManager::LoadMainMenu(std::string screenToShow /* = "startMenu"*/) {
	this->unloadPreviousScene();
	std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "mainMenu.uiscene";
	this->mainMenuTouchHandler = new MainMenuTouchHandlers();
	UiSceneLoader::LoadUiSceneFromUiSceneFile(pathToTestUiScene.c_str(), this->mainMenuTouchHandler);

	// Post load init
	this->mainMenuTouchHandler->createMissionMenu();
	SINGLETONS->GetLevelManager()->currentLevelIndex = -1;
	ENGINE->GetSceneGraph3D()->Pause();

	this->backdrop = (UiElement*)ObjectRegistry::GetObjectByName(BACKDROP);
	this->backdrop->SetVisible(false);

	if(screenToShow == START_MENU) {
		this->mainMenuTouchHandler->openStartMenu();
	} else if(screenToShow == CONTRACT) {
		this->mainMenuTouchHandler->openContractMenu();
	} else if(screenToShow == PARALLAX) {
		this->mainMenuTouchHandler->openMissionMenu(SINGLETONS->GetLevelManager()->GetCurrentContract());
	} else {
		UiElement* screen = (UiElement*)ObjectRegistry::GetObjectByName(screenToShow);
		VERIFY(screen != nullptr, "screen to show is not null");
		screen->SetVisible(true);
	}
	this->createMenuAudioSource();
}

void MenuManager::LoadGameMenu(std::string screenToShow /*= "inGame"*/) {
	this->unloadPreviousScene();
	std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "gameUi.uiscene";
	this->gameUiTouchHandler = new GameUiTouchHandlers();
	UiSceneLoader::LoadUiSceneFromUiSceneFile(pathToTestUiScene.c_str(), this->gameUiTouchHandler);

	this->backdrop = (UiElement*)ObjectRegistry::GetObjectByName(BACKDROP);
	glm::vec4 backdropColor = this->backdrop->GetSpriteColor();
	this->backdrop->SetSpriteColor(backdropColor);
	this->backdrop->SetVisible(false);
	UiElement* screen = (UiElement*)ObjectRegistry::GetObjectByName(screenToShow);
	VERIFY(screen != nullptr, "screen to show is not null");
	screen->SetVisible(true);
	this->createMenuAudioSource();

	if(this->loadScreen == nullptr) {
		this->loadScreen = (UiElement*)ObjectRegistry::GetObjectByName("loadScreen");
	}
	if(this->inGameTimer == nullptr) {
		this->inGameTimer = (UiElement*)ObjectRegistry::GetObjectByName("ingame_time_text");
	}
	if(this->inGameTreasure == nullptr) {
		this->inGameTreasure = (UiElement*)ObjectRegistry::GetObjectByName("ingame_loot_text");
	}
}

void MenuManager::LoadLevel(int levelIndex) {
	if(levelIndex < SINGLETONS->GetLevelManager()->GetNumLevelsInCurrentMission()) {
		this->showLoadScreen(levelIndex); // We only need a load screen when we are loading a new level
		
	} else {
		this->LoadMainMenu(PARALLAX);
	}

}



void MenuManager::TweenOutUiObject(UiObject* element) {
	if(element != nullptr) {
		float halfWidth = ((float)element->GetWidth()) / 2.0f;
		float halfHeight = ((float)element->GetHeight()) / 2.0f;

		float halfScreenWidth = ((float)FRAMEWORK->GetDeviceProperties()->GetWidthPixels()) / 2.0f;
		float halfScreenHeight= ((float)FRAMEWORK->GetDeviceProperties()->GetHeightPixels()) / 2.0f;
		
		glm::vec3 screenCenter = glm::vec3(halfScreenWidth - halfWidth, -halfScreenHeight + halfHeight, 0.0f);
		glm::vec3 offScreen = glm::vec3(halfScreenWidth - halfWidth, -1.0f * FRAMEWORK->GetDeviceProperties()->GetHeightPixels(), 0.0f);

		this->backdrop->SetVisible(true);
		ENGINE->GetTween()->TweenToNumber(BACKDROP_MAX_ALPHA, 0.0f, .5f, INTERPOLATION_TYPE_LINEAR, true, false, true, "backDropFade", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, NULL, menuManagerNumberTweenCallback);
		
		screenCenter.z = element->GetZOrder();
		offScreen.z = element->GetZOrder();

		element->GetTransform()->SetPosition(screenCenter);

		ENGINE->GetTween()->TweenPosition(element->GetId(),
										  screenCenter,
										  offScreen,
										  0.5f,
										  false,
										  INTERPOLATION_TYPE_LINEAR);
	}
}

void MenuManager::TweenInUiObject(UiObject* element) {
	if(element != nullptr) {
		float halfWidth = ((float)element->GetWidth()) / 2.0f;
		float halfHeight = ((float)element->GetHeight()) / 2.0f;
		
		float halfScreenWidth = ((float)FRAMEWORK->GetDeviceProperties()->GetWidthPixels()) / 2.0f;
		float halfScreenHeight= ((float)FRAMEWORK->GetDeviceProperties()->GetHeightPixels()) / 2.0f;
		
		glm::vec3 screenCenter = glm::vec3(halfScreenWidth - halfWidth, -halfScreenHeight + halfHeight, 0.0f);
		glm::vec3 offScreen = glm::vec3(halfScreenWidth - halfWidth, -768.0f, 0.0f);

		SINGLETONS->GetMenuManager()->backdrop->SetVisible(true);
		ENGINE->GetTween()->TweenToNumber(0.0f, BACKDROP_MAX_ALPHA, .5f, INTERPOLATION_TYPE_LINEAR, true, false, true, "backDropFade", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, NULL, menuManagerNumberTweenCallback);
				
		
		screenCenter.z = element->GetZOrder();
		offScreen.z = element->GetZOrder();
		
		element->GetTransform()->SetPosition(offScreen);

		ENGINE->GetTween()->TweenPosition(element->GetId(),
										  offScreen,
										  screenCenter,
										  0.5f,
										  false,
										  INTERPOLATION_TYPE_LINEAR);
	}
	
}
void MenuManager::CenterUiObject(UiObject* element) {
	if(element != nullptr) {
		float halfWidth = ((float)element->GetWidth()) / 2.0f;
		float halfHeight = ((float)element->GetHeight()) / 2.0f;
		
		float halfScreenWidth = ((float)FRAMEWORK->GetDeviceProperties()->GetWidthPixels()) / 2.0f;
		float halfScreenHeight= ((float)FRAMEWORK->GetDeviceProperties()->GetHeightPixels()) / 2.0f;
		
		glm::vec3 screenCenter = glm::vec3(halfScreenWidth - halfWidth, -halfScreenHeight + halfHeight, 0.0f);
		
		screenCenter.z = element->GetZOrder();
		element->GetTransform()->SetPosition(screenCenter);
		element->SetVisible(true);

		this->backdrop->SetVisible(true);
		glm::vec4 backdropColor = this->backdrop->GetSpriteColor();
		backdropColor.a = BACKDROP_MAX_ALPHA;
		this->backdrop->SetSpriteColor(backdropColor);
	}
	
}

void MenuManager::PlayBGM(std::string key) {
	// Background music is looped by default
	this->menuBGMSource->Play(key, true);
}

void MenuManager::PauseBGM() {
	this->menuBGMSource->Pause();
}

void MenuManager::StopBGM() {
	this->menuBGMSource->Stop();
}

void MenuManager::PlaySFX(std::string key) {
	this->menuSFXSource->Play(key);
}

void MenuManager::unloadPreviousScene() {
	UiSceneLoader::UnloadCurrentUiScene();
	this->mainMenuTouchHandler = nullptr;
	this->gameUiTouchHandler = nullptr;
	this->loadScreen = nullptr;
	this->inGameTimer = nullptr;
	this->inGameTreasure = nullptr;
}

void MenuManager::showLoadScreen(int levelIndex) {
	if(this->loadScreen == nullptr) {
		this->loadScreen = (UiElement*)ObjectRegistry::GetObjectByName("loadScreen");
	}
	this->loadScreen->SetVisible(true);
	
	glm::vec4 loadScreenColor = this->loadScreen->GetSpriteColor();
	loadScreenColor.a = 0.0f;
	this->loadScreen->SetSpriteColor(loadScreenColor);
	this->loadStartTime = ENGINE->GetTimer()->GetHighResAbsoluteTime();
	MessageData1S1I1F* userParams = new MessageData1S1I1F();
	userParams->i = levelIndex;
	ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, 0.25f, INTERPOLATION_TYPE_LINEAR, true, false, true, "loadScreenFadeIn", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, userParams, menuManagerNumberTweenCallback);
		

	
}

void MenuManager::hideLoadScreen() {
	if(this->loadScreen != nullptr) {
		//ENGINE->GetTween()->TweenToNumber(1.0f, 0.0f, .5f, INTERPOLATION_TYPE_LINEAR, true, false, true, "backDropFade", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, NULL, menuManagerNumberTweenCallback);
		
	}
	if(this->gameUiTouchHandler != nullptr) {
		UiObject* startButton = (UiObject*)ObjectRegistry::GetObjectByName("preMenuStart");
		startButton->SetVisible(true);
		float halfScreenWidth = ((float)FRAMEWORK->GetDeviceProperties()->GetWidthPixels()) / 2.0f;
		glm::vec3 startPos = startButton->GetTransform()->GetPositionWorld();
		startButton->GetTransform()->SetPositionWorld(halfScreenWidth - startButton->GetWidth() / 2.0f, startPos.y, startPos.z);
		ENGINE->GetTween()->TweenToNumber(1.0f, 0.0f, 1.5f, INTERPOLATION_TYPE_LINEAR, true, false, true, "loadScreenFadeOut", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, nullptr, menuManagerNumberTweenCallback);
	
		SINGLETONS->GetMenuManager()->UpdateTimer(0);
		SINGLETONS->GetMenuManager()->UpdateLoot(0);
		//ENGINE->GetTween()->TweenToNumber(0.0f, 0.1f, .2f, INTERPOLATION_TYPE_LINEAR, true, false, true, "delayPreMenuOpen", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, NULL, menuManagerNumberTweenCallback);
	}
}

void MenuManager::createMenuAudioSource() {
	std::string audioDir = "audio/";

	GameObject* bgmObj = new GameObject(ENGINE->GetSceneGraphUi());
	this->menuBGMSource = bgmObj->AddComponent<AudioSource>();
	this->menuBGMSource->SetVolume(0.1f);
	this->menuBGMSource->SetSourceIs3D(false);
	this->menuBGMSource->SetPlayOnlyWhenVisible(false);

	GameObject* sfxObj = new GameObject(ENGINE->GetSceneGraphUi());
	this->menuSFXSource = sfxObj->AddComponent<AudioSource>();
	this->menuSFXSource->SetSourceIs3D(false);
	this->menuSFXSource->SetPlayOnlyWhenVisible(false);
	this->menuSFXSource->LoadAudioClip(BUTTON_CLICK_SFX, audioDir + BUTTON_CLICK_ASSET);
}

void MenuManager::UpdateMenuWithMastery(std::string menuName, int levelIndex) {
	std::string menuPrefix;
	if(menuName == POST_GAME_WIN_MENU) {
		menuPrefix = "postGame_";
	} else if( menuName == PRE_GAME_MENU) {
		menuPrefix = "preGame_";
	} else if(menuName == PAUSE_MENU){
		menuPrefix = "pause_";
	} else {
		ASSERT(false, "\n %s does not support the mastery system", menuName.c_str());
		return;
	}
	LevelData* levelData = SINGLETONS->GetLevelManager()->GetLevelData(SINGLETONS->GetLevelManager()->GetCurrentMission(), levelIndex);
	LevelScores scores = SINGLETONS->GetMasteryManager()->GetLevelScores(levelIndex);
	UiElement* menuRoot = (UiElement*)ObjectRegistry::GetObjectByName(menuName);
	for(ObjectIdType id : menuRoot->GetChildren()) {
		UiElement* child = (UiElement*)ENGINE->GetSceneGraphUi()->GetGameObjectById(id);
		if(child->GetName() == menuPrefix + "bonus_value") {
			std::string text;
			switch(levelData->bonus) {
				case LevelBonus::Time:
					text = StringUtilities::ConvertIntToString(levelData->bonusValue) + " OR LESS";
					break;
				case LevelBonus::Kills:
					if(levelData->bonusValue > 0) {
						text = StringUtilities::ConvertIntToString(levelData->bonusValue) + " OR LESS";
					} else {
						text = StringUtilities::ConvertIntToString(levelData->bonusValue);
					}
					break;
				case LevelBonus::Alerts:
					if(levelData->bonusValue > 0) {
						text = StringUtilities::ConvertIntToString(levelData->bonusValue) + " OR LESS";
					} else {
						text = StringUtilities::ConvertIntToString(levelData->bonusValue);
					}
					break;
				case LevelBonus::Money:
					text = StringUtilities::ConvertIntToString(levelData->bonusValue) + " OR MORE";
					break;
				default:
					text = "there is no bonus";
					break;
			}
			child->ChangeText(text);
		} else if (child->GetName() == menuPrefix + "mission_name") {
			child->ChangeText(levelData->name);
		} else if (child->GetName() == menuPrefix + "mission_Type") {
			child->ChangeText(levelData->description);
		} else if (child->GetName() == menuPrefix + "bonus_icon") {
			child->SetSpriteTextureIndex(levelData->bonus);
		} else if (child->GetName() == menuPrefix + "bounty_value") {
			child->ChangeText(StringUtilities::ConvertIntToString(levelData->bounty));
		} else if (child->GetName() == menuPrefix + "take_value") {
			child->ChangeText("Testeroini");
		} else if (child->GetName() == menuPrefix + "bonus_completion") {
			if(levelData->completion == LevelCompletion::Completed_Bonus) {
				child->SetFontColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
				child->ChangeText("COMPLETED");
			} else {
				child->SetFontColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				child->ChangeText("INCOMPLETE");
			}
		} else if (child->GetName() == menuPrefix + "time_value") {
			int time = SINGLETONS->GetMasteryManager()->GetLevelTime();
			child->ChangeText(StringUtilities::ConvertIntToString(time));
		} else if (child->GetName() == menuPrefix + "time_total") {
			child->ChangeText(":30 x 5 = -500");
		} else if (child->GetName() == menuPrefix + "kill_value") {
			int kills = SINGLETONS->GetMasteryManager()->GetNumKills();
			child->ChangeText(StringUtilities::ConvertIntToString(kills));
		} else if (child->GetName() == menuPrefix + "kill_total") {
			child->ChangeText("4 x 7 = -1400");
		} else if (child->GetName() == menuPrefix + "alert_value") {
			int alerts = SINGLETONS->GetMasteryManager()->GetNumAlerts();
			child->ChangeText(StringUtilities::ConvertIntToString(alerts));
		} else if (child->GetName() == menuPrefix + "alert_total") {
			child->ChangeText("3 x -50 = -150");
		} else if (child->GetName() == menuPrefix + "loot_value") {	
			int loot = SINGLETONS->GetMasteryManager()->GetMoney();
			child->ChangeText(StringUtilities::ConvertIntToString(loot));
		} else if (child->GetName() == menuPrefix + "loot_total") {
			int loot = SINGLETONS->GetMasteryManager()->GetMoney();
			child->ChangeText(StringUtilities::ConvertIntToString(loot));
		} else if (child->GetName() == menuPrefix + "time_score") {
			std::string text;
			if(scores.time != -1) {
				text = StringUtilities::ConvertIntToString(scores.time);
			} else {
				text = "N/A";
			}
			child->ChangeText(text);
		} else if (child->GetName() == menuPrefix + "kill_score") {
			std::string text;
			if(scores.kills != -1) {
				text = StringUtilities::ConvertIntToString(scores.kills);
			} else {
				text = "N/A";
			}
			child->ChangeText(text);
		} else if (child->GetName() == menuPrefix + "alert_score") {
			std::string text;
			if(scores.alerts != -1) {
				text = StringUtilities::ConvertIntToString(scores.alerts);
			} else {
				text = "N/A";
			}
			child->ChangeText(text);
		} else if (child->GetName() == menuPrefix + "loot_score") {
			std::string text;
			if(scores.money != -1) {
				text = StringUtilities::ConvertIntToString(scores.money);
			} else {
				text = "N/A";
			}
			child->ChangeText(text);
		} 
	}

}

void MenuManager::loadLevel_internal(int levelIndex) {
	SINGLETONS->GetLevelManager()->LoadLevel(levelIndex);
}

void MenuManager::updatePreMenu(int levelIndex) {
	if(this->gameUiTouchHandler == nullptr) {
		SINGLETONS->GetMenuManager()->LoadGameMenu();
	}
	// Show the PreGame Menu ontop of the load screen
	this->CenterUiObject((UiObject*)ObjectRegistry::GetObjectByName("preMenu"));
	SINGLETONS->GetMasteryManager()->ResetTracking();
	this->UpdateMenuWithMastery("preMenu", levelIndex);
	
	this->backdrop->SetVisible(true);
	((UiObject*)ObjectRegistry::GetObjectByName("preMenuStart"))->SetVisible(false);
	((UiObject*)ObjectRegistry::GetObjectByName("preMenuEnd"))->SetVisible(false);
	
	glm::vec4 backdropColor = this->backdrop->GetSpriteColor();
	backdropColor.a = BACKDROP_MAX_ALPHA;
	this->backdrop->SetSpriteColor(backdropColor);


	this->loadScreen->SetVisible(true);
	glm::vec4 loadScreenColor = this->loadScreen->GetSpriteColor();
	loadScreenColor.a = 1.0f;
	this->loadScreen->SetSpriteColor(loadScreenColor);
}

void MenuManager::UpdateTimer(int newTime) {
	if(this->inGameTimer != nullptr) {
		inGameTimer->ChangeText(StringUtilities::ConvertIntToString(newTime));
	}
}

void MenuManager::UpdateLoot(int lootTotal) {
	if(this->inGameTreasure != nullptr) {
		inGameTreasure->ChangeText(StringUtilities::ConvertIntToString(lootTotal));
	}
}
