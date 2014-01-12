#ifndef I_TOUCH_TARGET_H
#define I_TOUCH_TARGET_H

#include "Vajra/Engine/Input/Definitions.h"

class ITouchTarget {
public:
	virtual bool TestTouch(Touch touch) = 0; // Test if target wants the touch, return true if touch is accepted
	virtual void OnTouch(int touchIndex) = 0; 
};

class IGameTouchTarget : public ITouchTarget {
public:
	IGameTouchTarget();
};

#endif //I_TOUCH_TARGET_H
 
