#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"

MessageHub::MessageHub() {
	this->init();
}

MessageHub::~MessageHub() {
	this->destroy();
}

void MessageHub::init() {
	this->currentlyAcceptingMessageCache = &(this->frontMessageCache);
	this->currentlyDrainingMessageCache = nullptr;
}

void MessageHub::destroy() {
}

void MessageHub::SendPointcastMessage(Message* message, ObjectIdType receiverId, ObjectIdType senderId /* = OBJECT_ID_INVALID */) {
	message->setReceiverId(receiverId);
	message->setSenderId(senderId);

	this->currentlyAcceptingMessageCache->PushMessageForReceipientId(message, receiverId);
}

void MessageHub::SendMulticastMessage(Message* message, ObjectIdType senderId /* = OBJECT_ID_INVALID */) {
	ASSERT(0, "Implement");
}


Message* MessageHub::RetrieveNextMessage(ObjectIdType id) {
	return this->currentlyDrainingMessageCache->PopMessageForReceipientId(id);
}


void MessageHub::DrainMessages() {
	this->currentlyAcceptingMessageCache = &this->backMessageCache;
	this->currentlyDrainingMessageCache = &this->frontMessageCache;
	this->drainMessageCache_internal();

	// Switch MessageCaches

	this->currentlyAcceptingMessageCache = &this->frontMessageCache;
	this->currentlyDrainingMessageCache = &this->backMessageCache;
	this->drainMessageCache_internal();

	this->currentlyDrainingMessageCache = nullptr;
}

void MessageHub::drainMessageCache_internal() {
	ObjectIdType receipientId = OBJECT_ID_INVALID;
	for (auto objectId_it = this->currentlyDrainingMessageCache->objectIdsWithPendingMessages.begin();
			objectId_it != this->currentlyDrainingMessageCache->objectIdsWithPendingMessages.end(); ++objectId_it) {

		if (objectId_it->second) {
			receipientId = objectId_it->first;

			Object* receipient = ObjectRegistry::GetObjectById(receipientId);
			if (receipient != nullptr) {
				receipient->HandleMessages();
			}
		}
		this->currentlyDrainingMessageCache->ClearMessagesForReceipientId(receipientId);
	}
	this->currentlyDrainingMessageCache->objectIdsWithPendingMessages.clear();
}
