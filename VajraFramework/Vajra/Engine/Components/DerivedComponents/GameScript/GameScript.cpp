#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/GameScript/GameScript.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/MeshRenderer.h"
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
	// TODO [Implement] Figure out if its better to add/remove subscription dynamically on play/pause/remove
	// subscribe the game script to the needed messages
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
	
	this->transform = nullptr;
	this->meshRenderer = nullptr;
}

void GameScript::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
	this->gameObject = nullptr;
	this->transform = nullptr;
	this->meshRenderer = nullptr;
}

// component accessors
Transform* GameScript::getTransform() {
	if(this->transform == nullptr){
		this->transform = (Transform*)gameObject->GetComponent<Transform>();
	}
	return this->transform;
}

MeshRenderer* GameScript::getMeshRenderer() {
	if(this->meshRenderer == nullptr){
		this->meshRenderer = (MeshRenderer*)gameObject->GetComponent<MeshRenderer>();
	}
	return this->meshRenderer;
}