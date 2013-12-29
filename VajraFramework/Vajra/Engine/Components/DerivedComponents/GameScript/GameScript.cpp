#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/GameScript/GameScript.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/Renderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"

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
	this->gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (this->gameObject != nullptr) {
		ASSERT(typeid(this->gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(this->gameObject).name(), this->gameObject->GetId(), typeid(GameObject*).name());
	}
	// subscribe the game script to the needed messages
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
	
	this->transform = nullptr;
	this->renderer = nullptr;
}

void GameScript::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
	this->gameObject = nullptr;
	this->transform = nullptr;
	this->renderer = nullptr;
}

// component accessors
Transform* GameScript::getTransform() {
	if(this->transform != nullptr){
		return this->transform;
	}
	return (this->transform = (Transform*)gameObject->GetComponent<Transform>());
}

Renderer* GameScript::getRenderer() {
	if(this->renderer != nullptr){
		return this->renderer;
	}
	return (this->renderer = (Renderer*)gameObject->GetComponent<Renderer>());
}