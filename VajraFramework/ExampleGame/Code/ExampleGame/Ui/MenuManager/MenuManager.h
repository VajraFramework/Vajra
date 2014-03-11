#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"

// Forward Declarations
class MainMenuTouchHandlers;
class GameUiTouchHandlers;
class MessageData1S1I1F;

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

	void TweenOutUiObject(UiObject*);
	void TweenInUiObject(UiObject*);
private:
	void init();
	void destroy();

	void unloadPreviousScene();

	void showLoadScreen();
	void hideLoadScreen();

	float loadStartTime;

	UiElement* loadScreen;
	UiElement*	backdrop;
	MainMenuTouchHandlers* mainMenuTouchHandler;
	GameUiTouchHandlers* gameUiTouchHandler;

	static ComponentIdType componentTypeId;

	friend void menuManagerNumberTweenCallback(float /* fromNumber */, float /* toNumber */, float /*currentNumber*/, std::string /*tweenClipName*/, MessageData1S1I1F* /*userParams*/);
};

#endif //MENU_MANAGER_H
