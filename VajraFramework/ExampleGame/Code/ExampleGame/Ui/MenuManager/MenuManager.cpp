#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/GameConstants/GameConstants.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "ExampleGame/Ui/MenuManager/MenuDefinitions.h"
#include "ExampleGame/Ui/MenuManager/MenuManager.h"
#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"

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

ComponentIdType MenuManager::componentTypeId = COMPONENT_TYPE_ID_LEVEL_MANAGER;

#define MIN_LOAD_TIME 0.3f
#define BACKDROP "popUpBack"
void menuManagerNumberTweenCallback(float /* fromNumber */, float toNumber , float currentNumber, std::string tweenClipName, MessageData1S1I1F* /*userParams*/) {
	if(tweenClipName == "extraLoadTime") {
		SINGLETONS->GetMenuManager()->hideLoadScreen();
	} else if(tweenClipName == "backDropFade") {
		glm::vec4 backdropColor = SINGLETONS->GetMenuManager()->backdrop->GetSpriteColor();
		backdropColor.a = currentNumber;
		SINGLETONS->GetMenuManager()->backdrop->SetSpriteColor(backdropColor);
		if(currentNumber == 0 && toNumber == 0) {
			SINGLETONS->GetMenuManager()->backdrop->SetVisible(false);
		}
	} else if (tweenClipName == "delayPreMenuOpen") {
		UiObject* preMenuScreen = (UiObject*)ObjectRegistry::GetObjectByName("preMenu");
		VERIFY(preMenuScreen != nullptr, "preMenuScreen to show is not null");
		preMenuScreen->SetVisible(true);
		SINGLETONS->GetMenuManager()->TweenInUiObject(preMenuScreen);
	}

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
				if(loadTime < GetFloatGameConstant(GAME_CONSTANT_min_load_time)) {
					float extraLoadTime = GetFloatGameConstant(GAME_CONSTANT_min_load_time) - loadTime;
					ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, extraLoadTime, INTERPOLATION_TYPE_LINEAR, false, false, false, "extraLoadTime", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, NULL, menuManagerNumberTweenCallback);
				} else {
					this->hideLoadScreen();
				}
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
	backdropColor.a = 0;
	this->backdrop->SetSpriteColor(backdropColor);
	this->backdrop->SetVisible(false);
	UiElement* screen = (UiElement*)ObjectRegistry::GetObjectByName(screenToShow);
	VERIFY(screen != nullptr, "screen to show is not null");
	screen->SetVisible(true);
	this->createMenuAudioSource();
}

void MenuManager::LoadLevel(int levelIndex) {
	// If were are in the mainMenu
	if(this->gameUiTouchHandler == nullptr) {
		this->LoadGameMenu();
	}

	if(levelIndex < SINGLETONS->GetLevelManager()->GetNumLevelsInCurrentMission()) {
		this->showLoadScreen(); // We only need a load screen when we are loading a new level
		SINGLETONS->GetLevelManager()->LoadLevel(levelIndex);
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
		ENGINE->GetTween()->TweenToNumber(0.7f, 0.0f, .5f, INTERPOLATION_TYPE_LINEAR, true, false, true, "backDropFade", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, NULL, menuManagerNumberTweenCallback);
		
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
		ENGINE->GetTween()->TweenToNumber(0.0f, 0.7f, .5f, INTERPOLATION_TYPE_LINEAR, true, false, true, "backDropFade", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, NULL, menuManagerNumberTweenCallback);
				
		
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
}

void MenuManager::showLoadScreen() {
	if(this->loadScreen == nullptr) {
		this->loadScreen = new UiElement(ENGINE->GetSceneGraphUi());
		ENGINE->GetSceneGraphUi()->GetRootGameObject()->AddChild(this->loadScreen->GetId());
		std::vector<std::string> pathsToTextures;
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "loading.png");
		this->loadScreen->InitSprite(FRAMEWORK->GetDeviceProperties()->GetWidthPixels(), FRAMEWORK->GetDeviceProperties()->GetHeightPixels(), "ustshdr", pathsToTextures, false);
		this->loadScreen->SetPosition(0.0f, 0.0f);
		this->loadScreen->SetZOrder(50);
	}
	this->loadScreen->SetVisible(true);
	this->loadStartTime = ENGINE->GetTimer()->GetHighResAbsoluteTime();
}

void MenuManager::hideLoadScreen() {
	if(this->loadScreen != nullptr) {
		this->loadScreen->SetVisible(false);
	}
	ENGINE->GetSceneGraph3D()->Resume();
	if(this->gameUiTouchHandler != nullptr) {
		ENGINE->GetTween()->TweenToNumber(0.0f, 0.1f, .2f, INTERPOLATION_TYPE_LINEAR, true, false, true, "delayPreMenuOpen", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_Ui, NULL, menuManagerNumberTweenCallback);
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
