#ifndef ANDROID_INPUT_SENDER_H
#define ANDROID_INPUT_SENDER_H

#ifdef PLATFORM_ANDROID

#include "Vajra/Engine/Input/Input.h"

// Note: these must stay in sync with their counterpart definitions in InputManager.java
#define TOUCH_EVENT_TYPE_INVALID -1
#define TOUCH_EVENT_TYPE_DOWN     0
#define TOUCH_EVENT_TYPE_UP       1
#define TOUCH_EVENT_TYPE_MOVE     2
#define TOUCH_EVENT_TYPE_CANCEL   3

namespace AndroidInputSender {
	void touchEventOccurred(int id, float x, float y, int touchEventType);
};

#endif // PLATFORM_ANDROID

#endif // ANDROID_INPUT_SENDER_H
