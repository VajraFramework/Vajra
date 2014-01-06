#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Platforms/AndroidInputSender.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

#ifdef PLATFORM_ANDROID

void AndroidInputSender::touchEventOccurred(int id, float x, float y, int touchEventType) {
	// FRAMEWORK->GetLogger()->dbglog("\nTouch occurred, %d: %f, %f: %d", id, x, y, touchEventType);

	switch (touchEventType) {
	case TOUCH_EVENT_TYPE_DOWN:
		{
			ENGINE->GetInput()->AddTouch(id, x, y);
		} break;

	case TOUCH_EVENT_TYPE_UP:
		{
			ENGINE->GetInput()->UpdateTouch(id, x, y, TouchPhase::Ended);
		} break;

	case TOUCH_EVENT_TYPE_CANCEL:
		{
			ENGINE->GetInput()->UpdateTouch(id, x, y, TouchPhase::Cancelled);
		} break;

	case TOUCH_EVENT_TYPE_MOVE:
		{
			ENGINE->GetInput()->UpdateTouch(id, x, y, TouchPhase::Moved);
		} break;

	case TOUCH_EVENT_TYPE_INVALID:
	default:
		{
			ASSERT(0, "Invalid touch event type: %d", touchEventType);
		} break;
	}

}


#endif // PLATFORM_ANDROID
