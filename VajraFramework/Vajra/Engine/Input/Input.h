#ifndef INPUT_H
#define INPUT_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/Input/ITouchTarget.h"
#include "Libraries/glm/glm.hpp"

#if PLATFORM_DESKTOP
// Include GLFW
#include <GL/glfw.h>
#endif

#include <iostream>
#include <vector>
#include <map>

enum TouchPhase
{
	Began,
	Ended,
	Moved,
	Stationary,
	Cancelled
};

enum GestureState {
	GestureState_Inactive,
	GestureState_Start ,
	GestureState_Changed,
	GestureState_End,
	GestureState_Cancelled
};

// TODO [Implement] Change this to use pools of Touch objects
struct Touch {
private:
	int uId;
public:
	int fingerId;
	glm::vec2 pos;
	glm::vec2 prevPos;
	TouchPhase phase;

friend class Input;
};

struct Pinch {
public:	
	float scale;
	float velocity;
	GestureState gestureState = GestureState::GestureState_Inactive;
};

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

    void testTouchTargets(int index);
	
    Pinch framePinch;
    Pinch asyncPinch;
    
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
