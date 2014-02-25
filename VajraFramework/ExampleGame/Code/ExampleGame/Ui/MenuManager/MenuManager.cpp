#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "ExampleGame/Ui/MenuManager/MenuManager.h"
#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"


ComponentIdType MenuManager::componentTypeId = COMPONENT_TYPE_ID_LEVEL_MANAGER;

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
		case MESSAGE_TYPE_LEVEL_LOADED:
			this->hideLoadScreen();
			break;
		default:
			break;
	}
}

void MenuManager::init() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_LEVEL_LOADED, this->GetTypeId(), false);
	this->LoadMainMenu("startMenu");
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
	
	UiElement* screen = (UiElement*)ObjectRegistry::GetObjectByName(screenToShow);
	VERIFY(screen != nullptr, "screen to show is not null");
	screen->SetVisible(true);
}

void MenuManager::LoadGameMenu(std::string screenToShow /*= "inGame"*/) {
	this->unloadPreviousScene();
	std::string pathToTestUiScene = FRAMEWORK->GetFileSystemUtils()->GetDeviceUiScenesResourcesPath() + "gameUi.uiscene";
	this->gameUiTouchHandler = new GameUiTouchHandlers();
	UiSceneLoader::LoadUiSceneFromUiSceneFile(pathToTestUiScene.c_str(), this->gameUiTouchHandler);

	UiElement* screen = (UiElement*)ObjectRegistry::GetObjectByName(screenToShow);
	VERIFY(screen != nullptr, "screen to show is not null");
	screen->SetVisible(true);
}

void MenuManager::LoadLevel(int levelIndex) {
	// If were are in the mainMenu
	if(this->gameUiTouchHandler == nullptr) {
		this->LoadGameMenu();
	}

	if(levelIndex < SINGLETONS->GetLevelManager()->NumLevels()) {
		if(levelIndex != SINGLETONS->GetLevelManager()->GetCurrentLevelIndex()) {
			this->showLoadScreen(); // We only need a load screen when we are loading a new level
		}
		SINGLETONS->GetLevelManager()->LoadLevel(levelIndex);
	} else {
		this->LoadMainMenu();
	}

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
		this->loadScreen->InitSprite(FRAMEWORK->GetDeviceProperties()->GetWidthPixels(), FRAMEWORK->GetDeviceProperties()->GetHeightPixels(), "uscshdr", glm::vec4(0.8f, 0.4f, 0.8f, 1.0f));
		this->loadScreen->SetPosition(0.0f, 0.0f);
		this->loadScreen->SetZOrder(5);
	}
	this->loadScreen->SetVisible(true);
}

void MenuManager::hideLoadScreen() {
	ASSERT(this->loadScreen != nullptr, "Load screen is not null");
	if(this->loadScreen != nullptr) {
		this->loadScreen->SetVisible(false);
	}
}