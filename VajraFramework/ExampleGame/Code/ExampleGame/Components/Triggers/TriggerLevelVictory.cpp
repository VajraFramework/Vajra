//
//  TriggerLevelVictory.cpp
//  Created by Matt Kaufmann on 02/19/14.
//

#include "ExampleGame/Components/Triggers/TriggerLevelVictory.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"

TriggerLevelVictory::TriggerLevelVictory() : Triggerable() {
	this->init();
}

TriggerLevelVictory::TriggerLevelVictory(Object* object_) : Triggerable(object_) {
	this->init();
}

TriggerLevelVictory::~TriggerLevelVictory() {
	this->destroy();
}

void TriggerLevelVictory::init() {
	this->type = TRIGGER_TYPE_ALL;
}

void TriggerLevelVictory::destroy() {

}

void TriggerLevelVictory::onSwitchActivated() {
	MessageChunk victoryMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	victoryMessage->SetMessageType(MESSAGE_TYPE_ON_END_CONDITIONS_MET);
	victoryMessage->messageData.iv1.x = 1; // Player won
	ENGINE->GetMessageHub()->SendMulticastMessage(victoryMessage, this->GetObject()->GetId());
}
