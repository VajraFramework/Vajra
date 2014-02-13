//
//  BaseSwitch.cpp
//  Created by Matt Kaufmann on 02/11/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Switches/BaseSwitch.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/Timer/Timer.h"

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
	this->isLocked     = false;
	this->resetTime    = 0.0f;
	this->resetCounter = 0.0f;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void BaseSwitch::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void BaseSwitch::SetSwitchType(std::string typeStr) {
	this->type = ConvertStringToSwitchType(typeStr);
}

void BaseSwitch::HandleMessage(MessageChunk messageChunk) {
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_FRAME_EVENT:
			this->update();
			break;
	}
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

void BaseSwitch::update() {
	bool condition = this->isConditionMet();
	float dt = ENGINE->GetTimer()->GetDeltaFrameTime();

	if (!this->isActive) {
		// Turn the switch on if its condition has been met and its state is not locked
		if (!this->isLocked && condition) {
			this->setActiveState(true);
		}
	}
	else {
		// If the switch is active, then its behavior depends on its type
		switch (this->type) {
			case SWITCH_TYPE_TOGGLE:              // Changes state each time switch condition is met
				if (!this->isLocked && condition) {
					this->setActiveState(false);
				}
				break;

			case SWITCH_TYPE_CONTINUOUS:          // Remains on for as long as condition is met
				if (!condition) {
					this->setActiveState(false);
				}
				break;

			case SWITCH_TYPE_RESET_ON_ACTIVATE:   // Remains on for a set period of time, then turns off
				this->resetCounter += dt;
				if (this->resetCounter >= this->resetTime) {
					this->setActiveState(false);
					this->resetCounter = 0.0f;
				}
				break;

			case SWITCH_TYPE_RESET_ON_DEACTIVATE: // Once turned on, turns off again after a period of time with the condition not being met
				if (condition) {
					this->resetCounter = 0.0f;
				}
				else {
					this->resetCounter += dt;
					if (this->resetCounter >= this->resetTime) {
						this->setActiveState(false);
						this->resetCounter = 0.0f;
					}
				}
				break;

			default: // SWITCH_TYPE_ONCE
				break;
		}
	}

	this->isLocked = condition; // The switch remains locked for as long as the condition remains true
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
