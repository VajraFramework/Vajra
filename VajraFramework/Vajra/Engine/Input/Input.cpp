#include <algorithm>

#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Utilities/Utilities.h"

#if PLATFORM_DESKTOP
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
#if PLATFORM_DESKTOP
	glfwSetMousePosCallback(cursorPosCallback);
#endif
}

void Input::destroy() {
}

bool touchOver(const Touch &touch) {
	return (touch.phase == TouchPhase::Ended || touch.phase == TouchPhase::Cancelled);
}

void Input::updateInput() { 
#if PLATFORM_DESKTOP
	this->updateMouseButton();
#endif
	// Populate frame touches with all input that has been asynchronously collected
	this->frameTouches = this->asyncTouches;

	// Send out input messages
	for(std::vector<Touch>::iterator it = this->frameTouches.begin(); it != this->frameTouches.end(); ++it) {
    	MessageChunk touchMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
		touchMessage->SetMessageType(MESSAGE_TYPE_TOUCH_OFF_UI);
		ENGINE->GetMessageHub()->SendMulticastMessage(touchMessage, this->GetId());
	}
	// Remove all touches that have ended or been cancelled
	this->asyncTouches.erase( std::remove_if(this->asyncTouches.begin(), this->asyncTouches.end(), touchOver), this->asyncTouches.end());

	for(std::vector<Touch>::iterator it = this->asyncTouches.begin(); it != this->asyncTouches.end(); ++it) {
    	it->phase = TouchPhase::Stationary;
	}
	//logTouches();

	this->framePinch = this->asyncPinch;

	// Set the async pinch as inactive so UpdatePinch will know to update it
	this->asyncPinch.gestureState = GestureState::GestureState_Inactive;

	// If the gesture state is greater than end it has either ended, been cancelled or another gesture has been detected
	if(this->asyncPinch.gestureState >= GestureState::GestureState_End) {
		this->asyncPinch.gestureState = GestureState::GestureState_Inactive;
	}
	if(this->framePinch.gestureState != GestureState::GestureState_Inactive) {
		// Raise the pinch gesture event
		MessageChunk pinchGestureMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
		pinchGestureMessage->SetMessageType(MESSAGE_TYPE_PINCH_GESTURE);
		ENGINE->GetMessageHub()->SendMulticastMessage(pinchGestureMessage, this->GetId());
	}	
}

Touch Input::GetTouch(int index) {
	ASSERT((unsigned int)index < this->frameTouches.size(), "Index is greater than current number of touches");
	return this->frameTouches[index];
}

void Input::AddTouch(int uId, float startX, float startY, TouchPhase phase) {
	Touch t;
	t.uId = uId;
	t.pos.x = startX;
	t.pos.y = startY;
	t.prevPos = t.pos;
	t.phase = phase;
	t.fingerId = this->asyncTouches.size();
	this->asyncTouches.push_back(t);

	// Send a message to the UI that a touch started
}

void Input::UpdateTouch(int uId, float curX, float curY, TouchPhase phase) {
	for(std::vector<Touch>::iterator it = this->asyncTouches.begin(); it != this->asyncTouches.end(); ++it) {
		if(it->uId == uId) {
			it->prevPos = it->pos;
			it->pos.x = curX;
			it->pos.y = curY;
			it->phase = phase;
			break;
		}
	}
}

void Input::UpdatePinch(float scale, float velocity, GestureState gestureState) {
	this->asyncPinch.scale = scale;
	this->asyncPinch.velocity = velocity;

	if(this->asyncPinch.gestureState != GestureState::GestureState_Start)
		this->asyncPinch.gestureState = gestureState;
}

void Input::logTouches() {
	if(this->GetTouchCount() > 0)
		FRAMEWORK->GetLogger()->dbglog("TOUCH LOG \n");
	for(std::vector<Touch>::iterator it = this->frameTouches.begin(); it != this->frameTouches.end(); ++it) {
		FRAMEWORK->GetLogger()->dbglog("Touch id: %i pos: (%f, %f) %i \n", it->fingerId, it->pos.x, it->pos.y, (int)it->phase);
	}
}

#if PLATFORM_DESKTOP
void Input::updateMouseButton() {
	// Grab the left click state and make it act like a single touch
	int button = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
	if(ENGINE->GetInput()->GetTouchCount() == 0) {
		if(button == GLFW_PRESS) {
			ENGINE->GetInput()->AddTouch(0, this->mouseX, this->mouseY);
		}
	}
	else {
		if(button == GLFW_RELEASE) {
			ENGINE->GetInput()->UpdateTouch(0, this->mouseX, this->mouseY, TouchPhase::Ended);
		}
	}
}

void Input::cursorPosUpdate(int x, int y) {
	this->mouseX = x;
	this->mouseY = y;
	this->UpdateTouch(0, x, y, TouchPhase::Moved);
}
#endif
