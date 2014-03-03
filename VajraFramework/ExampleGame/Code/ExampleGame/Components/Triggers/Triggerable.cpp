//
//  Triggerable.cpp
//  Created by Matt Kaufmann on 02/11/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Switches/DecalGenerator.h"
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
	this->isToggled      = false;

	this->decalRef = nullptr;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_SWITCH_ACTIVATED, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SWITCH_DEACTIVATED, this->GetTypeId(), true);
}

void Triggerable::destroy() {
	this->UnsubscribeToAllSwitches();
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void Triggerable::SetTriggerType(std::string typeStr) {
	this->type = ConvertStringToTriggerType(typeStr);
}

void Triggerable::SetToggleState(bool toggle) {
	this->isToggled = toggle;
}

void Triggerable::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_SWITCH_ACTIVATED:
			this->markAsActive(messageChunk->GetSenderId());
			break;

		case MESSAGE_TYPE_SWITCH_DEACTIVATED:
			this->markAsInactive(messageChunk->GetSenderId());
			break;
	}
}

void Triggerable::SubscribeToMySwitch() {
	this->SubscribeToSwitchObject(this->GetObject()->GetId());
}

void Triggerable::SubscribeToParentSwitch() {
	this->SubscribeToSwitchObject(this->GetObject()->GetParentId());
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
				if (switchComp->IsActive()) {
					this->markAsActive(switchId);
				}
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
		if (switchObj != nullptr) {
			BaseSwitch* switchComp = switchObj->GetComponent<BaseSwitch>();
			if (switchComp != nullptr) {
				switchComp->RemoveSubscriber(this->GetObject()->GetId());
			}
		}
		this->subscriptions.erase(it);
		this->markAsInactive(switchId);
	}
	else {
		FRAMEWORK->GetLogger()->dbglog("Warning: Trying to unsubscribe for unfound subscription to switch: %d by triggerable: %d", switchId, this->GetObject()->GetId());
	}
}

void Triggerable::UnsubscribeToAllSwitches() {
	while (this->subscriptions.size() > 0) {
		this->UnsubscribeToSwitchObject(this->subscriptions.front());
	}
}

void Triggerable::markAsActive(ObjectIdType switchId) {
	auto iter = std::find(this->activeSwitches.begin(), this->activeSwitches.end(), switchId);
	if (iter == this->activeSwitches.end()) {
		int prevNumActive = this->activeSwitches.size();
		int numSwitches = this->subscriptions.size();

		// Add the switch's id to the list.
		this->activeSwitches.push_back(switchId);
		int numActive = this->activeSwitches.size();

		switch (this->type) {
			case TRIGGER_TYPE_ALL:
				if ((prevNumActive < numSwitches) && (numActive >= numSwitches)) {
					this->toggleState();
				}
				else if ((numSwitches == 0) && (prevNumActive == 0)) {
					this->toggleState();
				}
				break;

			case TRIGGER_TYPE_ANY:
				if ((prevNumActive < 1) && (numActive >= 1)) {
					this->toggleState();
				}
				break;

			default:
				break;
		}
	}
}

void Triggerable::markAsInactive(ObjectIdType switchId) {
	auto iter = std::find(this->activeSwitches.begin(), this->activeSwitches.end(), switchId);
	if (iter != this->activeSwitches.end()) {
		int prevNumActive = this->activeSwitches.size();
		int numSwitches = this->subscriptions.size();

		// Remove the switch's id from the list.
		this->activeSwitches.erase(iter);
		int numActive = this->activeSwitches.size();

		switch (this->type) {
		case TRIGGER_TYPE_ALL:
			if ((prevNumActive >= numSwitches) && (numActive < numSwitches)) {
				this->toggleState();
			}
			else if ((numSwitches == 0) && (prevNumActive > 0)) {
				this->toggleState();
			}
			break;

		case TRIGGER_TYPE_ANY:
			if ((prevNumActive >= 1) && (numActive < 1)) {
				this->toggleState();
			}
			break;

		default:
			break;
		}
	}
}

void Triggerable::toggleState() {
	if (this->isToggled) {
		this->onSwitchToggled(false);
		this->onSwitchDeactivated();
	}
	else {
		this->onSwitchToggled(true);
		this->onSwitchActivated();
	}
	this->isToggled = !this->isToggled;
}

void Triggerable::setDecalType(std::string decalType_) {
	this->decalType = decalType_;

	GameObject* gameObjectRef = (GameObject*)this->GetObject();
	ASSERT(gameObjectRef->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");

	this->decalRef = DecalGenerator::GetDecalFromDecalType(this->decalType, gameObjectRef->GetParentSceneGraph());
	gameObjectRef->AddChild(this->decalRef->GetId());
}

void Triggerable::SetDecalType(std::string decalType) {
	this->setDecalType(decalType);
}
