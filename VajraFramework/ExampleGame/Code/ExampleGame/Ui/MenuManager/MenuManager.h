#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"

// Forward Declarations
class MainMenuTouchHandlers;
class GameUiTouchHandlers;
//[[COMPONENT]]//
class MenuManager : public Component {	
public:
	MenuManager();
	MenuManager(Object* object_);
	~MenuManager();

	void HandleMessage(MessageChunk messageChunk);

	void LoadMainMenu(std::string defaultScreen = "startMenu");
	void LoadGameMenu(std::string defaultScreen = "inGame");

	void LoadLevel(int /*levelIndex*/);
private:
	void init();
	void destroy();

	void unloadPreviousScene();

	void showLoadScreen();
	void hideLoadScreen();

	UiElement* loadScreen;

	MainMenuTouchHandlers* mainMenuTouchHandler;
	GameUiTouchHandlers* gameUiTouchHandler;

	static ComponentIdType componentTypeId;

	//friend class LevelManager;
};

#endif //MENU_MANAGER_H
