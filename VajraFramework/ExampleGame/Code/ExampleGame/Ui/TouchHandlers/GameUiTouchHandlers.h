#ifndef GAMEUI_TOUCH_HANDLERS_H
#define GAMEUI_TOUCH_HANDLERS_H

#include "Vajra/Engine/Ui/UiTouchHandlers/UiTouchHandlers.h"

class GameUiTouchHandlers : public UiTouchHandlers {
public:
	GameUiTouchHandlers();
	// @Override
	virtual void OnTouchDownHandlers(UiObject* uiObject, Touch touch);
	virtual void OnTouchMoveHandlers(UiObject* uiObject, Touch touch);
	virtual void OnTouchUpHandlers  (UiObject* uiObject, Touch touch);
private:
};

#endif // GAMEUI_TOUCH_HANDLERS_H
