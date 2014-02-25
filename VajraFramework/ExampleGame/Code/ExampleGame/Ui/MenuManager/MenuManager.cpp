#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Ui/MenuManager/MenuManager.h"
#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/MainMenuTouchHandlers.h"

#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
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
}

void MenuManager::init() {
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

	// Post load init


	UiElement* screen = (UiElement*)ObjectRegistry::GetObjectByName(screenToShow);
	VERIFY(screen != nullptr, "screen to show is not null");
	screen->SetVisible(true);
}

void MenuManager::unloadPreviousScene() {
	UiSceneLoader::UnloadCurrentUiScene();
	this->mainMenuTouchHandler = nullptr;
	this->gameUiTouchHandler = nullptr;
}