#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/GameScript/GameScript.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"

unsigned int GameScript::componentTypeId = COMPONENT_TYPE_ID_GAME_SCRIPT;

GameScript::GameScript() : Component() {
	this->init();
}

GameScript::GameScript(Object* object_) : Component(object_) {
	this->init();
}

GameScript::~GameScript() {
	this->destroy();
}

void GameScript::HandleMessage(Message* message) {
	switch(message->GetMessageType()) {
		case MESSAGE_TYPE_FRAME_EVENT:
			this->update();
	default:
		break;
	}
}

void GameScript::init() {
	// subscribe the game script to the needed messages
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void GameScript::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}