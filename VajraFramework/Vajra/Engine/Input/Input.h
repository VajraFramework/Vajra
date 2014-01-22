#ifndef INPUT_H
#define INPUT_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/Input/Definitions.h"
#include "Vajra/Engine/Input/ITouchTarget.h"

#include "Libraries/glm/glm.hpp"

#if PLATFORM_DESKTOP
// Include GLFW
#include <GL/glfw.h>
#endif

#include <iostream>
#include <vector>
#include <map>

class Input : public Object {

static const int MAX_TOUCHES = 1;

public:
	~Input();

	Touch GetTouch(int index);
	void AddTouch(int uId, float startX, float startY, TouchPhase phase = TouchPhase::Began);
	void UpdateTouch(int uId, float curX, float curY, TouchPhase phase);
	int GetTouchCount() { return this->frameTouches.size(); }

	Pinch GetPinch() { return this->framePinch; }
	void UpdatePinch(float scale, float velocity, GestureState gestureState);
	bool HasPinchEnded() { return this->framePinch.gestureState >= GestureState::GestureState_End; }

	void UpdateLongPress(float x, float y, GestureState gestureState);

	void AddGameTouchTarget(IGameTouchTarget* newTarget);
private:
	Input();
	void init();
	void destroy();

	void updateInput();
	void logTouches();

    std::vector<Touch> frameTouches;
    std::vector<Touch> asyncTouches;

    ITouchTarget* currentTouchTarget;
    std::vector<ITouchTarget*> gameTouchTargets;

    void testTouchTargets(Touch touch);
	
    Pinch framePinch;
    Pinch asyncPinch;

    LongPress frameLongPress;
    LongPress asyncLongPress;

    int nextFingerId;
    
#if PLATFORM_DESKTOP
    int mouseX;
    int mouseY;
    void updateMouseButton();
public:
    void cursorPosUpdate(int x, int y);
#endif
	friend class Engine;
};

#endif // INPUT_H
