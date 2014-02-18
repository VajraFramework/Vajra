//
//  MultiplexSwitch.cpp
//  Created by Matt Kaufmann on 02/16/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Switches/MultiplexSwitch.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"

MultiplexSwitch::MultiplexSwitch() : BaseSwitch() {
	this->init();
}

MultiplexSwitch::MultiplexSwitch(Object* object_) : BaseSwitch(object_) {
	this->init();
}

MultiplexSwitch::~MultiplexSwitch() {
	this->destroy();
}

void MultiplexSwitch::init() {
	this->triggerType    = TRIGGER_TYPE_ALL;
	this->activeSwitches = 0;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_SWITCH_ACTIVATED, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SWITCH_DEACTIVATED, this->GetTypeId(), true);
}

void MultiplexSwitch::destroy() {

}

void MultiplexSwitch::SetSwitchType(std::string typeStr) {
	this->type = ConvertStringToSwitchType(typeStr);
}

void MultiplexSwitch::SetResetTime(float t) {
	BaseSwitch::SetResetTime(t);
}

void MultiplexSwitch::SetTriggerType(std::string typeStr) {
	this->triggerType = ConvertStringToTriggerType(typeStr);
}

void MultiplexSwitch::HandleMessage(MessageChunk messageChunk) {
	BaseSwitch::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_SWITCH_ACTIVATED:
			this->activeSwitches++;
			if (this->shouldTriggerActivate()) {
				this->setConditionState(true);
				//this->onSwitchToggled(true);
				//this->onSwitchActivated();
			}
			break;

		case MESSAGE_TYPE_SWITCH_DEACTIVATED:
			this->activeSwitches--;
			if (this->shouldTriggerDeactivate()) {
				this->setConditionState(false);
				//this->onSwitchToggled(false);
				//this->onSwitchDeactivated();
			}
			break;
	}
}

void MultiplexSwitch::SubscribeToParentSwitch() {
	this->SubscribeToSwitchObject(this->GetObject()->GetParentId());
}

void MultiplexSwitch::SubscribeToSwitchObject(ObjectIdType switchId) {
	auto it = std::find(this->subscriptions.begin(), this->subscriptions.end(), switchId);
	if (it == this->subscriptions.end()) {
		GameObject* switchObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(switchId);
		ASSERT(switchObj != nullptr, "Object exists with id %d", switchId);
		if (switchObj != nullptr) {
			BaseSwitch* switchComp = switchObj->GetComponent<BaseSwitch>();
			ASSERT(switchComp != nullptr, "Object with id %d has BaseSwitch component", switchId);
			if (switchComp != nullptr) {
				switchComp->AddSubscriber(this->GetObject()->GetId());
				this->subscriptions.push_back(switchId);
			}
		}
	}
	else {
		FRAMEWORK->GetLogger()->dbglog("Duplicate subscription to switch: %d by triggerable: %d", switchId, this->GetObject()->GetId());
	}
}

void MultiplexSwitch::UnsubscribeToSwitchObject(ObjectIdType switchId) {
	auto it = std::find(this->subscriptions.begin(), this->subscriptions.end(), switchId);
	if (it != this->subscriptions.end()) {
		GameObject* switchObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(switchId);
		ASSERT(switchObj != nullptr, "Object exists with id %d", switchId);
		if (switchObj != nullptr) {
			BaseSwitch* switchComp = switchObj->GetComponent<BaseSwitch>();
			ASSERT(switchComp != nullptr, "Object with id %d has BaseSwitch component", switchId);
			if (switchComp != nullptr) {
				switchComp->RemoveSubscriber(this->GetObject()->GetId());
				this->subscriptions.erase(it);
			}
		}
	}
	else {
		FRAMEWORK->GetLogger()->dbglog("Warning: Trying to unsubscribe for unfound subscription to switch: %d by triggerable: %d", switchId, this->GetObject()->GetId());
	}
}

void MultiplexSwitch::onSwitchToggled(bool /*switchState*/) {

}

void MultiplexSwitch::onSwitchActivated() {

}

void MultiplexSwitch::onSwitchDeactivated() {

}

bool MultiplexSwitch::shouldTriggerActivate() {
	switch (this->triggerType) {
		case TRIGGER_TYPE_ALL:
			return (this->activeSwitches >= (int)this->subscriptions.size());

		case TRIGGER_TYPE_ANY:
			return (this->activeSwitches > 0);

		default:
			return false;
	}
}

bool MultiplexSwitch::shouldTriggerDeactivate() {
	switch (this->triggerType) {
		case TRIGGER_TYPE_ALL:
			return (this->activeSwitches < (int)this->subscriptions.size());

		case TRIGGER_TYPE_ANY:
			return (this->activeSwitches <= 0);

		default:
			return false;
	}
}
