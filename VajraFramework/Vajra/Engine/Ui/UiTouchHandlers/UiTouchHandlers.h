#ifndef UI_TOUCH_HANDLERS_H
#define UI_TOUCH_HANDLERS_H

#include "Vajra/Common/Objects/Declarations.h"
#include "Vajra/Engine/Input/Definitions.h"

#include <map>
#include <string>

// Forward Declarations:
class UiObject;

class UiTouchHandlers {
public:
	virtual void OnTouchDownHandlers(UiObject* uiObject, Touch touch) = 0;
	virtual void OnTouchMoveHandlers(UiObject* uiObject, Touch touch) = 0;
	virtual void OnTouchUpHandlers  (UiObject* uiObject, Touch touch) = 0;

	std::map<std::string /* uiObjectName */, ObjectIdType> uiSceneObjects;
private:
};

#endif // UI_TOUCH_HANDLERS_H
