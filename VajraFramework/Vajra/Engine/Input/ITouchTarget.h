#ifndef I_TOUCH_TARGET_H
#define I_TOUCH_TARGET_H

class ITouchTarget {
public:
	virtual bool TestTouch(int fingerId) = 0; // Test if target wants the touch, return true if touch is accepted
	virtual void OnTouch(int fingerId) = 0; 
};

class IGameTouchTarget : public ITouchTarget {
public:
	IGameTouchTarget();
};

#endif //I_TOUCH_TARGET_H
 