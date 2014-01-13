#include "Vajra/Common/Components/Component.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Logging/Logger.h"

ComponentIdType Component::componentTypeId = COMPONENT_TYPE_ID_DEFAULT;

Component::Component() {
	this->init();
}

Component::Component(Object* object_) {
	this->init(object_);
}

Component::~Component() {
	this->destroy();
}

void Component::init(Object* object_ /* = 0 */) {
	this->object = object_;
}

void Component::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void Component::HandleMessage(MessageChunk messageChunk) {
	switch(messageChunk->GetMessageType()) {

	case MESSAGE_TYPE_FRAME_EVENT:
		this->update();
		break;

	case MESSAGE_TYPE_SCENE_START:
		this->start();
		break;

	case MESSAGE_TYPE_SCENE_END:
		this->end();
		break;

	default:
		break;
	}
}

void Component::addSubscriptionToMessageType(MessageType messageType, ComponentIdType selfComponentId, bool onLocalObject) {
	if (this->GetObject() != nullptr) {
		this->GetObject()->SubscribeToMessageType(messageType, selfComponentId, onLocalObject);
	} else {
		FRAMEWORK->GetLogger()->dbglog("\nFailed to add subscription to messageType: %d because Object hasn't been assigned yet", messageType);
	}
}

void Component::removeSubscriptionToMessageType(MessageType messageType, ComponentIdType selfComponentId) {
	this->GetObject()->UnsubscribeToMessageType(messageType, selfComponentId);
}

void Component::removeSubscriptionToAllMessageTypes(ComponentIdType selfComponentId) {
	for (MessageType& messageType : this->messageTypesSubscribedTo) {
		this->removeSubscriptionToMessageType(messageType, selfComponentId);
	}
}
