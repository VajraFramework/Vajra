#ifndef TESTUISCNE_TOUCH_HANDLERS_H
#define TESTUISCNE_TOUCH_HANDLERS_H

#include "Vajra/Engine/Ui/UiTouchHandlers/UiTouchHandlers.h"

class TestUiSceneTouchHandlers : public UiTouchHandlers {
public:
	// @Override
	virtual void OnTouchDownHandlers(UiObject* uiObject, Touch touch);
	virtual void OnTouchMoveHandlers(UiObject* uiObject, Touch touch);
	virtual void OnTouchUpHandlers  (UiObject* uiObject, Touch touch);

private:
};

#endif // TESTUISCNE_TOUCH_HANDLERS_H
