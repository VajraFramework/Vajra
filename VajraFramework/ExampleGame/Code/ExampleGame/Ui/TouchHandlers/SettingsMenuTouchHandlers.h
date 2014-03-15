#ifndef SETTINGS_MENU_TOUCH_HANDLERS_H
#define SETTINGS_MENU_TOUCH_HANDLERS_H

#include "Vajra/Engine/Ui/UiTouchHandlers/UiTouchHandlers.h"

class UiElement;
class SettingsMenuTouchHandlers : public UiTouchHandlers {
public:
	SettingsMenuTouchHandlers();

	// @Override
	virtual void OnTouchDownHandlers(UiObject* uiObject, Touch touch);
	virtual void OnTouchMoveHandlers(UiObject* uiObject, Touch touch);
	virtual void OnTouchUpHandlers  (UiObject* uiObject, Touch touch);

private:

};


#endif // SETTINGS_MENU_TOUCH_HANDLERS_H
