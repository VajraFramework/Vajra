#ifndef UI_TOUCH_HANDLERS_H
#define UI_TOUCH_HANDLERS_H

#include "Vajra/Common/Objects/Declarations.h"
#include "Vajra/Engine/Input/Definitions.h"
#include "Vajra/Engine/MessageHub/Declarations.h"

#include <map>
#include <string>

// Forward Declarations:
class UiObject;
class GameObject;

class UiTouchHandlers {
public:
	UiTouchHandlers();
	~UiTouchHandlers();
	virtual void OnTouchDownHandlers(UiObject* uiObject, Touch touch) = 0;
	virtual void OnTouchMoveHandlers(UiObject* uiObject, Touch touch) = 0;
	virtual void OnTouchUpHandlers  (UiObject* uiObject, Touch touch) = 0;

	std::map<std::string /* uiObjectName */, ObjectIdType> uiSceneObjects;

	virtual void HandleMessageCallback(MessageChunk messageChunk);
protected:
	GameObject* eventForwarder;
};

#endif // UI_TOUCH_HANDLERS_H
