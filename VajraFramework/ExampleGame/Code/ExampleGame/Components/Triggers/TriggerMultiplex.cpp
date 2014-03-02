//
//  TriggerMultiplex.cpp
//  Created by Matt Kaufmann on 03/02/14.
//

#include "ExampleGame/Components/Triggers/TriggerMultiplex.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"

TriggerMultiplex::TriggerMultiplex() : Triggerable() {
	this->init();
}

TriggerMultiplex::TriggerMultiplex(Object* object_) : Triggerable(object_) {
	this->init();
}

TriggerMultiplex::~TriggerMultiplex() {
	this->destroy();
}

void TriggerMultiplex::init() {

}

void TriggerMultiplex::destroy() {

}

void TriggerMultiplex::SetTriggerType(std::string typeStr) {
	Triggerable::SetTriggerType(typeStr);
}

void TriggerMultiplex::SetToggleState(bool toggle) {
	Triggerable::SetToggleState(toggle);
}

void TriggerMultiplex::SubscribeToMySwitch() {
	ASSERT(false, "Object with TriggerMultiplex component is attempting to subscribe to itself");
}

void TriggerMultiplex::SubscribeToParentSwitch() {
	Triggerable::SubscribeToParentSwitch();
}

void TriggerMultiplex::SetDecalType(std::string decalType) {
	Triggerable::SetDecalType(decalType);
}

void TriggerMultiplex::onSwitchToggled(bool switchState) {
	if (this->isToggled != switchState) {
		MessageType msgType;
		if (switchState) { msgType = MESSAGE_TYPE_MULTIPLEX_ACTIVATED;   }
		else             { msgType = MESSAGE_TYPE_MULTIPLEX_DEACTIVATED; }

		ObjectIdType myId = this->GetObject()->GetId();
		MessageChunk switchMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
		switchMessage->SetMessageType(msgType);
		switchMessage->messageData.iv1.x = myId;
		ENGINE->GetMessageHub()->SendPointcastMessage(switchMessage, myId, myId);
	}
}
