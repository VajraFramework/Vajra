
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Utilities/Utilities.h"

#include <algorithm>

#if PLATFORM_DESKTOP
#include "Vajra/Engine/Timer/Timer.h"
// Global callback function for desktop input
void cursorPosCallback(int x, int y) {
	ENGINE->GetInput()->cursorPosUpdate(x, y);
}
#endif

Input::Input() {
}

Input::~Input() {
	this->destroy();
}

void Input::init() {
	this->nextFingerId = 0;
#if PLATFORM_DESKTOP
	glfwSetMousePosCallback(cursorPosCallback);
#endif
}

void Input::destroy() {
	this->currentTouchTarget = nullptr;
}

bool touchOver(const Touch &touch) {
	return (touch.phase == TouchPhase::Ended
			|| touch.phase == TouchPhase::Cancelled);
}

void Input::updateInput() {
#if PLATFORM_DESKTOP
	this->updateDesktopInput();
#endif
	// Populate frame touches with all input that has been asynchronously collected
	this->frameTouches = this->asyncTouches;

	// Send touches to the interested target
	for (unsigned int i = 0; i < this->frameTouches.size(); ++i) {
		if (this->currentTouchTarget != nullptr) {
			this->currentTouchTarget->OnTouchUpdate(i);
		}
	}

	// Remove all touches that have ended or been cancelled
	this->asyncTouches.erase(
			std::remove_if(this->asyncTouches.begin(), this->asyncTouches.end(),
					touchOver), this->asyncTouches.end());

	for (std::vector<Touch>::iterator it = this->asyncTouches.begin();
			it != this->asyncTouches.end(); ++it) {
		it->prevPos = it->pos;
		it->phase = TouchPhase::Stationary;
		it->timeDown += ENGINE->GetTimer()->GetDeltaFrameTime();
	}
	//logTouches();

	this->framePinch = this->asyncPinch;

	// Set the async pinch as inactive so UpdatePinch will know to update it
	this->asyncPinch.gestureState = GestureState::GestureState_Inactive;

	// If the gesture state is greater then end it has either ended, been cancelled or another gesture has been detected
	if (this->asyncPinch.gestureState >= GestureState::GestureState_End) {
		this->asyncPinch.gestureState = GestureState::GestureState_Inactive;
	}
	if (this->framePinch.gestureState != GestureState::GestureState_Inactive) {
		// Raise the pinch gesture event
		ENGINE->GetMessageHub()->SendMulticastMessage(MESSAGE_TYPE_PINCH_GESTURE, this->GetId());
	}
	
#ifdef LONG_PRESS
	this->frameLongPress = this->asyncLongPress;
	this->asyncLongPress.gestureState = GestureState::GestureState_Inactive;

	if (this->asyncLongPress.gestureState >= GestureState::GestureState_End) {
		this->asyncLongPress.gestureState = GestureState::GestureState_Inactive;
	}
	if (this->frameLongPress.gestureState
			!= GestureState::GestureState_Inactive) {
		// Raise the long press gesture event
		ENGINE->GetMessageHub()->SendMulticastMessage(MESSAGE_TYPE_LONG_PRESS_GESTURE, this->GetId());
	}
#endif
}

Touch Input::GetTouch(int index) {
	ASSERT((unsigned int)index < this->frameTouches.size(), "Index (%d) is lesser than current number of touches (%u)", index, this->frameTouches.size());
	return this->frameTouches[index];
}

void Input::AddTouch(int uId, float startX, float startY, TouchPhase phase) {
	Touch t;
	t.uId = uId;
	t.pos.x = startX;
	t.pos.y = startY;
	t.prevPos = t.pos;
	t.phase = phase;
	t.fingerId = this->nextFingerId;
	t.timeDown = 0.0f;
	// increase the unique finger id
	this->nextFingerId = (this->nextFingerId + 1) % MAX_TOUCHES;

	// add the touch
	this->asyncTouches.push_back(t);

	// Find correct touch target for touch using vector index
	this->testTouchTargets(t);

}

void Input::UpdateTouch(int uId, float curX, float curY, TouchPhase phase) {
	for (std::vector<Touch>::iterator it = this->asyncTouches.begin();
			it != this->asyncTouches.end(); ++it) {
		if (it->uId == uId) {
			it->prevPos = it->pos;
			it->pos.x = curX;
			it->pos.y = curY;
			if(it->phase != TouchPhase::Began) {
				it->phase = phase;
			}
			break;
		}
	}
}

void Input::UpdatePinch(float scale, float velocity,
		GestureState gestureState) {
	this->asyncPinch.scale = scale;
	this->asyncPinch.velocity = velocity;

	if (this->asyncPinch.gestureState != GestureState::GestureState_Start)
		this->asyncPinch.gestureState = gestureState;
}

#ifdef LONG_PRESS
void Input::UpdateLongPress(float x, float y, GestureState gestureState) {
	this->asyncLongPress.pos.x = x;
	this->asyncLongPress.pos.y = y;

	if (this->asyncLongPress.gestureState != GestureState::GestureState_Start) {
		this->asyncLongPress.gestureState = gestureState;
	}
}
#endif

void Input::AddGameTouchTarget(IGameTouchTarget* newTarget) {
	this->gameTouchTargets.push_back(newTarget);
}

void Input::logTouches() {
	if (this->GetTouchCount() > 0) {
		FRAMEWORK->GetLogger()->dbglog("TOUCH LOG \n");
		for(std::vector<Touch>::iterator it = this->frameTouches.begin(); it != this->frameTouches.end(); ++it) {
			FRAMEWORK->GetLogger()->dbglog("Touch id: %i pos: (%f, %f) %i \n", it->fingerId, it->pos.x, it->pos.y, (int)it->phase);
		}
	}
}

void Input::testTouchTargets(Touch touch) {
	// Test UIScene first
	if (ENGINE->GetSceneGraphUi()->TestTouchStart(touch)) {
		this->currentTouchTarget = ENGINE->GetSceneGraphUi();
		return;
	}

	// Test all game targets
	for(ITouchTarget* iTarget : this->gameTouchTargets) {
		if(iTarget->TestTouchStart(touch)) {
			this->currentTouchTarget = iTarget;
			return;
		}
	}

	// Test 3DScene last
	if(ENGINE->GetSceneGraph3D()->TestTouchStart(touch)) {
		this->currentTouchTarget = ENGINE->GetSceneGraph3D();
		return;
	}
}

#if PLATFORM_DESKTOP
#define GLFW_KEY_I 73
#define GLFW_KEY_O 79

float downTime = 0;
const float longPress = 1.0f;
float startX, startY;
void Input::updateDesktopInput() {
	// Grab the left click state and make it act like a single touch
	int button = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);

#ifdef LONG_PRESS
	if (this->frameLongPress.gestureState != GestureState::GestureState_Inactive
			&& this->frameLongPress.gestureState
					!= GestureState::GestureState_End) {
		if (button == GLFW_RELEASE) {
			this->UpdateLongPress(this->mouseX, this->mouseY,
					GestureState::GestureState_End);
		} else {
			this->UpdateLongPress(this->mouseX, this->mouseY,
					GestureState::GestureState_Changed);
		}
	} else
#endif
	{
		if (this->frameTouches.size() == 0) {
			if (button == GLFW_PRESS) {
				this->touchDown = true;
				downTime = 0;
				ENGINE->GetInput()->AddTouch(0, this->mouseX, this->mouseY);
				startX = this->mouseX;
				startY = this->mouseY;
			}
		} else {
			if (button == GLFW_RELEASE) {
				if (this->touchDown) {
					ENGINE->GetInput()->UpdateTouch(0, this->mouseX, this->mouseY, TouchPhase::Ended);
					this->touchDown = false;
				}
			}
			else {
				if(this->touchDown) {
#ifdef LONG_PRESS
					downTime += (float)ENGINE->GetTimer()->GetDeltaFrameTime();
					if(downTime >= longPress) {
						if((startX == this->mouseX) && (startY == this->mouseY)) {
							ENGINE->GetInput()->UpdateTouch(0, this->mouseX, this->mouseY, TouchPhase::Cancelled);
							this->UpdateLongPress(this->mouseX, this->mouseY, GestureState::GestureState_Start);
							this->touchDown = false;
						}
					}	
#endif
				}
			}
		}
	}
	//pinch
	int pinchIn = glfwGetKey(GLFW_KEY_I);
	int pinchOut = glfwGetKey(GLFW_KEY_O);

	if (pinchIn == GLFW_PRESS) {
		this->UpdatePinch(10.0f, 10.0f, GestureState::GestureState_Start);
	}
	if (pinchOut == GLFW_PRESS) {
		this->UpdatePinch(10.0f, -10.0f, GestureState::GestureState_Start);
	}
}

void Input::cursorPosUpdate(int x, int y) {
	this->mouseX = x;
	this->mouseY = y;
	if (this->frameTouches.size() == 0) {
		return;
	}
	this->UpdateTouch(0, x, y, TouchPhase::Moved);

}
#endif
