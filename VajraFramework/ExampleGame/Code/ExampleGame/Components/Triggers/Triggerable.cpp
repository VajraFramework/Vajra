//
//  Triggerable.cpp
//  Created by Matt Kaufmann on 02/11/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Switches/BaseSwitch.h"
#include "ExampleGame/Components/Triggers/Triggerable.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"

TriggerType ConvertStringToTriggerType(std::string str) {
	if (str == "All") {
		return TRIGGER_TYPE_ALL;
	}
	if (str == "Any") {
		return TRIGGER_TYPE_ANY;
	}
	return TRIGGER_TYPE_INVALID;
}

ComponentIdType Triggerable::componentTypeId = COMPONENT_TYPE_ID_TRIGGERABLE;

Triggerable::Triggerable() : Component() {
	this->init();
}

Triggerable::Triggerable(Object* object_) : Component(object_) {
	this->init();
}

Triggerable::~Triggerable() {
	this->destroy();
}

void Triggerable::init() {
	this->type           = TRIGGER_TYPE_ALL;
	this->activeSwitches = 0;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_SWITCH_ACTIVATED, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SWITCH_DEACTIVATED, this->GetTypeId(), true);
}

void Triggerable::destroy() {
	while (this->subscriptions.size() > 0) {
		this->UnsubscribeToSwitchObject(this->subscriptions.front());
	}
}

void Triggerable::SetTriggerType(std::string typeStr) {
	this->type = ConvertStringToTriggerType(typeStr);
}

void Triggerable::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_SWITCH_ACTIVATED:
			this->activeSwitches++;
			if (this->shouldTriggerActivate()) {
				this->onSwitchToggled(true);
				this->onSwitchActivated();
			}
			break;

		case MESSAGE_TYPE_SWITCH_DEACTIVATED:
			this->activeSwitches--;
			if (this->shouldTriggerDeactivate()) {
				this->onSwitchToggled(false);
				this->onSwitchDeactivated();
			}
			break;
	}
}

void Triggerable::SubscribeToMySwitch() {
	this->SubscribeToSwitchObject(this->GetObject()->GetId());
}

void Triggerable::SubscribeToSwitchObject(ObjectIdType switchId) {
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

void Triggerable::UnsubscribeToSwitchObject(ObjectIdType switchId) {
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

bool Triggerable::shouldTriggerActivate() {
	switch (this->type) {
		case TRIGGER_TYPE_ALL:
			return (this->activeSwitches >= (int)this->subscriptions.size());

		case TRIGGER_TYPE_ANY:
			return (this->activeSwitches > 0);

		default:
			return false;
	}
}

bool Triggerable::shouldTriggerDeactivate() {
	switch (this->type) {
		case TRIGGER_TYPE_ALL:
			return (this->activeSwitches < (int)this->subscriptions.size());

		case TRIGGER_TYPE_ANY:
			return (this->activeSwitches <= 0);

		default:
			return false;
	}
}
