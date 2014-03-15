#ifndef MAINMENU_TOUCH_HANDLERS_H
#define MAINMENU_TOUCH_HANDLERS_H

#include "Vajra/Engine/Ui/UiTouchHandlers/UiTouchHandlers.h"

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
	float startTouchX;
	float currentScreenX;

	std::vector<UiObject*> currentLevelButtons;
	UiElement* missionRoot;
	
	int currentMission;
	void parallaxScroll(UiObject* parallaxRoot, float touchX, bool touchEnd = false);
	void scrollToCurrentMission();

	std::vector<int> missionStartX;
	friend class MenuManager;
};

#endif // MAINMENU_TOUCH_HANDLERS_H
