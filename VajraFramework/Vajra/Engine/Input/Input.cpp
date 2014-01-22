#include <algorithm>

#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
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
	this->nextFingerId = 0;
#if PLATFORM_DESKTOP
	glfwSetMousePosCallback(cursorPosCallback);
#endif
}

void Input::destroy() {
	this->currentTouchTarget = nullptr;
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

	// Send touches to the interested target
	for(unsigned int i = 0; i < this->frameTouches.size(); ++i) {
    	if(this->currentTouchTarget != nullptr) {
			this->currentTouchTarget->OnTouchUpdate(i);
    	}
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

	// If the gesture state is greater then end it has either ended, been cancelled or another gesture has been detected
	if(this->asyncPinch.gestureState >= GestureState::GestureState_End) {
		this->asyncPinch.gestureState = GestureState::GestureState_Inactive;
	}
	if(this->framePinch.gestureState != GestureState::GestureState_Inactive) {
		// Raise the pinch gesture event
		MessageChunk pinchGestureMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
		pinchGestureMessage->SetMessageType(MESSAGE_TYPE_PINCH_GESTURE);
		ENGINE->GetMessageHub()->SendMulticastMessage(pinchGestureMessage, this->GetId());
	}	

	this->frameLongPress = this->asyncLongPress;
	if(this->asyncLongPress.gestureState >= GestureState::GestureState_End) {
			this->asyncLongPress.gestureState = GestureState::GestureState_Inactive;
	}
	if(this->frameLongPress.gestureState != GestureState::GestureState_Inactive) {
		// Raise the pinch gesture event
		MessageChunk longPressGestureMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
		longPressGestureMessage->SetMessageType(MESSAGE_TYPE_LONG_PRESS_GESTURE);
		ENGINE->GetMessageHub()->SendMulticastMessage(longPressGestureMessage, this->GetId());
	}	
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
	// increase the unique finger id
	this->nextFingerId = (this->nextFingerId + 1) % MAX_TOUCHES;

	// add the touch
	this->asyncTouches.push_back(t);

	// Find correct touch target for touch using vector index
	this->testTouchTargets(t);


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

void Input::UpdateLongPress(float x, float y, GestureState gestureState) {
	this->asyncLongPress.pos.x = x;
	this->asyncLongPress.pos.y = y;

	if(this->asyncLongPress.gestureState != GestureState::GestureState_Start) {
		this->asyncLongPress.gestureState = gestureState;
	}
}


void Input::AddGameTouchTarget(IGameTouchTarget* newTarget) {
	this->gameTouchTargets.push_back(newTarget);
}
void Input::logTouches() {
	if(this->GetTouchCount() > 0) {
		FRAMEWORK->GetLogger()->dbglog("TOUCH LOG \n");
		for(std::vector<Touch>::iterator it = this->frameTouches.begin(); it != this->frameTouches.end(); ++it) {
			FRAMEWORK->GetLogger()->dbglog("Touch id: %i pos: (%f, %f) %i \n", it->fingerId, it->pos.x, it->pos.y, (int)it->phase);
		}
	}
}

void Input::testTouchTargets(Touch touch) {
	// Test UIScene first
	if(ENGINE->GetSceneGraphUi()->TestTouchStart(touch)) {
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
