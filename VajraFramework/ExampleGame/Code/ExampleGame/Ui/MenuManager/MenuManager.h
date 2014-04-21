#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"

// Forward Declarations
class MainMenuTouchHandlers;
class GameUiTouchHandlers;
class MessageData1S1I1F;
class AudioSource;

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
	void CenterUiObject(UiObject*);

	AudioSource* GetBGMSource() { return this->menuBGMSource; }
	void PlayBGM(std::string key);
	void PauseBGM();
	void StopBGM();

	AudioSource* GetSFXSource() { return this->menuSFXSource; }
	void PlaySFX(std::string key);
private:
	void init();
	void destroy();

	void unloadPreviousScene();

	void showLoadScreen(int levelIndex);
	void hideLoadScreen();

	void createMenuAudioSource();

	float loadStartTime;

	UiElement* loadScreen;
	UiElement*	backdrop;
	MainMenuTouchHandlers* mainMenuTouchHandler;
	GameUiTouchHandlers* gameUiTouchHandler;
	AudioSource* menuBGMSource;
	AudioSource* menuSFXSource;

	static ComponentIdType componentTypeId;

	friend void menuManagerNumberTweenCallback(float /* fromNumber */, float /* toNumber */, float /*currentNumber*/, std::string /*tweenClipName*/, MessageData1S1I1F* /*userParams*/);
};

#endif //MENU_MANAGER_H
