//
//  BaseSwitch.cpp
//  Created by Matt Kaufmann on 02/11/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Switches/BaseSwitch.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"

SwitchType ConvertStringToSwitchType(std::string str) {
	if      (str == "Once") {
		return SWITCH_TYPE_ONCE;
	}
	else if (str == "Toggle") {
		return SWITCH_TYPE_TOGGLE;
	}
	else if (str == "Continuous") {
		return SWITCH_TYPE_CONTINUOUS;
	}
	else if (str == "ResetOnActivate") {
		return SWITCH_TYPE_RESET_ON_ACTIVATE;
	}
	else if (str == "ResetOnDeactivate") {
		return SWITCH_TYPE_RESET_ON_DEACTIVATE;
	}
	return SWITCH_TYPE_INVALID;
}

ComponentIdType BaseSwitch::componentTypeId = COMPONENT_TYPE_ID_BASE_SWITCH;

void baseSwitchNumberTweenCallback(float /* fromNumber */, float /* toNumber */, float /*currentNumber*/, std::string /*tweenClipName*/, MessageData1S1I1F* userParams) {
	GameObject* caller = ENGINE->GetSceneGraph3D()->GetGameObjectById(userParams->i);

	// Make sure the switch is still around
	if (caller != nullptr) {
		BaseSwitch* switchComp = caller->GetComponent<BaseSwitch>();
		ASSERT(switchComp != nullptr, "BaseSwitch Tween called by object %d without BaseSwitch component", userParams->i);
		if (switchComp != nullptr) {
			switchComp->setActiveState(userParams->f > 0.0f);
		}
	}
}

BaseSwitch::BaseSwitch() : Component() {
	this->init();
}

BaseSwitch::BaseSwitch(Object* object_) : Component(object_) {
	this->init();
}

BaseSwitch::~BaseSwitch() {
	this->destroy();
}

void BaseSwitch::init() {
	this->type         = SWITCH_TYPE_CONTINUOUS;
	this->isActive     = false;
	this->resetTime    = 0.0f;
}

void BaseSwitch::destroy() {

}

void BaseSwitch::SetSwitchType(std::string typeStr) {
	this->type = ConvertStringToSwitchType(typeStr);
}

void BaseSwitch::SetResetTime(float t) {
	this->resetTime = t;
}

void BaseSwitch::AddSubscriber(ObjectIdType subscriberId) {
	auto it = std::find(this->subscribers.begin(), this->subscribers.end(), subscriberId);
	if (it == this->subscribers.end()) {
		this->subscribers.push_back(subscriberId);
	}
	else {
		FRAMEWORK->GetLogger()->dbglog("Duplicate subscription to switch: %d by object id: %d", this->GetObject()->GetId(), subscriberId);
	}
}

void BaseSwitch::RemoveSubscriber(ObjectIdType subscriberId) {
	auto it = std::find(this->subscribers.begin(), this->subscribers.end(), subscriberId);
	if (it != this->subscribers.end()) {
		this->subscribers.erase(it);
	}
	else {
		FRAMEWORK->GetLogger()->dbglog("Warning: Trying to unsubscribe for unfound subscription to switch: %d by object id: %d", this->GetObject()->GetId(), subscriberId);
	}
}

void BaseSwitch::setConditionState(bool state) {
	std::string tweenName = "SwitchTimeout";
	tweenName += this->GetObject()->GetId();
	MessageData1S1I1F* userParams;

	switch (this->type) {
		case SWITCH_TYPE_ONCE:                // Once turned on, stays on permanently
			if (state) {
				this->setActiveState(true);
			}
			break;

		case SWITCH_TYPE_TOGGLE:              // Changes state each time switch condition is met
			if (state) {
				this->setActiveState(!this->isActive);
			}
			break;

		case SWITCH_TYPE_CONTINUOUS:          // Remains on for as long as condition is met
			this->setActiveState(state);
			break;

		case SWITCH_TYPE_RESET_ON_ACTIVATE:   // Remains on for a set period of time, then turns off
			if (state && !this->isActive) {
				this->setActiveState(true);
				userParams = new MessageData1S1I1F();
				userParams->i = this->GetObject()->GetId();
				userParams->f = -1.0f;
				ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, this->resetTime, false, false, false, tweenName, userParams, baseSwitchNumberTweenCallback);
			}
			break;

		case SWITCH_TYPE_RESET_ON_DEACTIVATE: // Once turned on, turns off again after a period of time with the condition not being met
			if (state) {
				this->setActiveState(true);
				ENGINE->GetTween()->CancelNumberTween(tweenName);
			}
			else if (this->isActive) {
				userParams = new MessageData1S1I1F();
				userParams->i = this->GetObject()->GetId();
				userParams->f = -1.0f;
				ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, this->resetTime, false, false, false, tweenName, userParams, baseSwitchNumberTweenCallback);
			}
			break;

		default:
			break;
	}
}

void BaseSwitch::setActiveState(bool state) {
	if (this->isActive != state) {
		MessageType msgType;
		if (state) { msgType = MESSAGE_TYPE_SWITCH_ACTIVATED;   }
		else       { msgType = MESSAGE_TYPE_SWITCH_DEACTIVATED; }

		ObjectIdType myId = this->GetObject()->GetId();
		for (auto iter = this->subscribers.begin(); iter != this->subscribers.end(); ++iter) {
			MessageChunk switchMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			switchMessage->SetMessageType(msgType);
			switchMessage->messageData.iv1.x = myId;
			ENGINE->GetMessageHub()->SendPointcastMessage(switchMessage, this->GetObject()->GetId(), myId);
		}

		this->isActive = state;
	}
}
