#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"

#include <algorithm>

MessageHub::MessageHub() {
}

MessageHub::~MessageHub() {
	this->destroy();
}

void MessageHub::init() {
	this->currentlyAcceptingMessageCacheRef = &(this->frontMessageCache);
	this->currentlyDrainingMessageCacheRef = nullptr;
}

void MessageHub::destroy() {
}

void MessageHub::SendPointcastMessage(MessageChunk messageChunk, ObjectIdType receiverId, ObjectIdType senderId /* = OBJECT_ID_INVALID */) {
	messageChunk->setSenderId(senderId);

	this->currentlyAcceptingMessageCacheRef->PushMessageForReceipientId(messageChunk, receiverId);
}

void MessageHub::SendMulticastMessage(MessageChunk messageChunk, ObjectIdType senderId /* = OBJECT_ID_INVALID */) {
	unsigned int numMsgTypes = this->subscribersForMessageType.size();
	if (numMsgTypes <= messageChunk->GetMessageType()) { return; }
	unsigned int numSubscribers = this->subscribersForMessageType[messageChunk->GetMessageType()].size();
	for (unsigned int i = 0; i < numSubscribers; ++i) {
		this->SendPointcastMessage(messageChunk, this->subscribersForMessageType[messageChunk->GetMessageType()][i], senderId);
	}
}

void MessageHub::SubscribeToMessageType(MessageType messageType, ObjectIdType subscriberId) {
	while (messageType >= this->subscribersForMessageType.size()) {
		this->subscribersForMessageType.push_back(std::vector<ObjectIdType>(0));
	}

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
		FRAMEWORK->GetLogger()->dbglog("Warning: Trying to unsubscribe for unfound subscription for messageType:%d by object id: %d", messageType, subscriberId);
	}
}

MessageChunk MessageHub::RetrieveNextMessage(ObjectIdType id, bool& returnValueIsValid) {
	MessageChunk messageChunk = this->currentlyDrainingMessageCacheRef->PopMessageForReceipientId(id, returnValueIsValid);
	return messageChunk;
}


void MessageHub::drainMessages() {
	this->currentlyAcceptingMessageCacheRef = &this->backMessageCache;
	this->currentlyDrainingMessageCacheRef = &this->frontMessageCache;
	this->drainMessageCache_internal();

	// Switching MessageCaches

	this->currentlyAcceptingMessageCacheRef = &this->frontMessageCache;
	this->currentlyDrainingMessageCacheRef = &this->backMessageCache;
	this->drainMessageCache_internal();

	this->currentlyDrainingMessageCacheRef = nullptr;
}

void MessageHub::drainMessageCache_internal() {
	ObjectIdType receipientId = OBJECT_ID_INVALID;
	for (auto objectId_it = this->currentlyDrainingMessageCacheRef->objectIdsWithPendingMessages.begin();
			objectId_it != this->currentlyDrainingMessageCacheRef->objectIdsWithPendingMessages.end(); ++objectId_it) {

		if (objectId_it->second) {
			receipientId = objectId_it->first;

			Object* receipient = ObjectRegistry::GetObjectById(receipientId);
			if (receipient != nullptr) {
				ASSERT(receipient->GetId() > OBJECT_ID_INVALID, "Valid object id (%d) should have been %d", receipient->GetId(), receipientId);
				receipient->HandleMessages();
			} else {
				FRAMEWORK->GetLogger()->dbglog("\nWarning: Trying to deliver message to missing object of id %d", receipientId);
			}
		}
		this->currentlyDrainingMessageCacheRef->ClearMessagesForReceipientId(receipientId);
	}
	this->currentlyDrainingMessageCacheRef->objectIdsWithPendingMessages.clear();
}
