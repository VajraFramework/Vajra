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
	this->addSubscriptionToMessageType(MESSAGE_TYPE_MULTIPLEX_ACTIVATED, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_MULTIPLEX_DEACTIVATED, this->GetTypeId(), true);
}

void MultiplexSwitch::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void MultiplexSwitch::SetSwitchType(std::string typeStr) {
	this->type = ConvertStringToSwitchType(typeStr);
}

void MultiplexSwitch::SetResetTime(float t) {
	BaseSwitch::SetResetTime(t);
}

void MultiplexSwitch::HandleMessage(MessageChunk messageChunk) {
	BaseSwitch::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_MULTIPLEX_ACTIVATED:
			this->setConditionState(true);
			break;

		case MESSAGE_TYPE_MULTIPLEX_DEACTIVATED:
			this->setConditionState(false);
			break;
	}
}
