#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"

#include <algorithm>

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
	unsigned int numSubscribers = this->subscribersForMessageType[message->GetMessageType()].size();
	for (unsigned int i = 0; i < numSubscribers; ++i) {
		// TODO [Cleanup] Too many allocations here. Pools maybe? Or better yet, define messages as const, so that we can just send the same message to all of them (reference counted), somehow
		Message* newMesage = new Message(*message);
		this->SendPointcastMessage(newMesage, this->subscribersForMessageType[message->GetMessageType()][i], senderId);
	}
}

void MessageHub::SubscribeToMessageType(MessageType messageType, ObjectIdType subscriberId) {
	auto it = std::find(this->subscribersForMessageType[messageType].begin(), this->subscribersForMessageType[messageType].end(), subscriberId);
	if (it == this->subscribersForMessageType[messageType].end()) {
		this->subscribersForMessageType[messageType].push_back(subscriberId);
	} else {
		FRAMEWORK->GetLogger()->dbglog("Duplicate subscription for messageType:%d by object id: %d", messageType, subscriberId);
	}
}

void MessageHub::UnsubscribeToMessageType(MessageType messageType, ObjectIdType subscriberId) {
	auto it = std::find(this->subscribersForMessageType[messageType].begin(), this->subscribersForMessageType[messageType].end(), subscriberId);
	if (it != this->subscribersForMessageType[messageType].end()) {
		this->subscribersForMessageType[messageType].erase(it);
	} else {
		FRAMEWORK->GetLogger()->dbglog("Trying to unsubscribe for unfound subscription for messageType:%d by object id: %d", messageType, subscriberId);
	}
}

Message* MessageHub::RetrieveNextMessage(ObjectIdType id) {
	return this->currentlyDrainingMessageCache->PopMessageForReceipientId(id);
}


void MessageHub::drainMessages() {
	this->currentlyAcceptingMessageCache = &this->backMessageCache;
	this->currentlyDrainingMessageCache = &this->frontMessageCache;
	this->drainMessageCache_internal();

	// Switching MessageCaches

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
