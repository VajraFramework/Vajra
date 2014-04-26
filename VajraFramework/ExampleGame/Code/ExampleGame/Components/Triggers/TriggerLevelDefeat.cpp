//
//  TriggerLevelDefeat.cpp
//  Created by Matt Kaufmann on 02/19/14.
//

#include "ExampleGame/Components/Triggers/TriggerLevelDefeat.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "ExampleGame/Ui/MenuManager/MenuDefinitions.h"
#include "ExampleGame/Ui/MenuManager/MenuManager.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"

TriggerLevelDefeat::TriggerLevelDefeat() : Triggerable() {
	this->init();
}

TriggerLevelDefeat::TriggerLevelDefeat(Object* object_) : Triggerable(object_) {
	this->init();
}

TriggerLevelDefeat::~TriggerLevelDefeat() {
	this->destroy();
}

void TriggerLevelDefeat::init() {
	this->type = TRIGGER_TYPE_ANY;
}

void TriggerLevelDefeat::destroy() {

}

void TriggerLevelDefeat::onSwitchActivated() {
	MessageChunk victoryMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	victoryMessage->SetMessageType(MESSAGE_TYPE_ON_END_CONDITIONS_MET);
	victoryMessage->messageData.iv1.x = -1; // Player lost
	ENGINE->GetMessageHub()->SendMulticastMessage(victoryMessage, this->GetObject()->GetId());
	SINGLETONS->GetMenuManager()->PlayBGM(LEVEL_LOSE_BGM, false);
}
