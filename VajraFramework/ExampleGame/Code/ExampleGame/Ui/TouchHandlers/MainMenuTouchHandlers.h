#ifndef MAINMENU_TOUCH_HANDLERS_H
#define MAINMENU_TOUCH_HANDLERS_H

#include "Vajra/Engine/Ui/UiTouchHandlers/UiTouchHandlers.h"

#define START_MENU "startMenu"
#define OPTIONS_MENU "optionsMenu"
#define MISSION_MENU "missionMenu"
#define CONTRACT "contractRoot"
#define PARALLAX "parallaxRoot"
#define PARALLAX_FRONT "parallaxFront"
#define PARALLAX_MIDDLE "parallaxMid"
#define PARALLAX_BACK "parallaxBack"

class UiElement;
class MainMenuTouchHandlers : public UiTouchHandlers {
public:
	MainMenuTouchHandlers();

	// @Override
	virtual void OnTouchDownHandlers(UiObject* uiObject, Touch touch);
	virtual void OnTouchMoveHandlers(UiObject* uiObject, Touch touch);
	virtual void OnTouchUpHandlers  (UiObject* uiObject, Touch touch);

private:
	void createMissionMenu();

	void loadPips(int contractIndex);

	void displaySettings();
	void applySettings();
	
	void parallaxScroll(UiObject* parallaxRoot, float touchX, bool touchEnd = false);
	void scrollToCurrentMission();

	void openStartMenu();
	void openContractMenu();
	void openMissionMenu(int contractIndex);

	void goBackOneMenu();

	int levelToLoad;
	
	int currentMissionScreenIndex;
	int prevContractIndex;
	std::vector<UiElement*> parallaxScreens;
	std::vector<UiElement*> levelPips;

	std::vector<std::vector<UiObject*>> currentLevelButtons;
	UiElement* missionRoot;
	UiElement* contractRoot;
	UiElement* parallaxRoot;
	float startTouchX;
	float currentScreenX;

	std::vector<int> missionStartX;
	friend class MenuManager;
};

#endif // MAINMENU_TOUCH_HANDLERS_H
