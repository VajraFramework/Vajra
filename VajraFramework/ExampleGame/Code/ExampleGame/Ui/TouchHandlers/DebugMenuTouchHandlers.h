#ifdef DEBUG
#ifndef DEBUGMENU_TOUCH_HANDLERS_H
#define DEBUGMENU_TOUCH_HANDLERS_H

#include "Vajra/Engine/Ui/UiTouchHandlers/UiTouchHandlers.h"

class DebugMenuTouchHandlers : public UiTouchHandlers {
public:
	DebugMenuTouchHandlers();
	// @Override
	virtual void OnTouchDownHandlers(UiObject* uiObject, Touch touch);
	virtual void OnTouchMoveHandlers(UiObject* uiObject, Touch touch);
	virtual void OnTouchUpHandlers  (UiObject* uiObject, Touch touch);
private:
	float cameraSpeed = 2.0f;
	bool positionMode = true;
	glm::vec2 downPos;
};

#endif // DEBUGMENU_TOUCH_HANDLERS_H
#endif