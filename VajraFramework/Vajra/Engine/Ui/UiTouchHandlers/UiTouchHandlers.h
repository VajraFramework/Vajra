#ifndef UI_TOUCH_HANDLERS_H
#define UI_TOUCH_HANDLERS_H

#include "Vajra/Engine/Input/Definitions.h"

// Forward Declarations:
class UiObject;

class UiTouchHandlers {
public:
	virtual void OnTouchDownHandlers(UiObject* uiObject, Touch touch) = 0;
	virtual void OnTouchMoveHandlers(UiObject* uiObject, Touch touch) = 0;
	virtual void OnTouchUpHandlers  (UiObject* uiObject, Touch touch) = 0;

private:
};

#endif // UI_TOUCH_HANDLERS_H
