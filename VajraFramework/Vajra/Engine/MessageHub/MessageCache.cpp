#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/MessageHub/MessageCache.h"
#include "Vajra/Utilities/Utilities.h"

// TODO [Cleanup] Remove after debugging
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

#include <algorithm>

#define NUM_BUCKETS_IN_CACHE 100

MessageCache::MessageCache() {
	this->init();
}

MessageCache::~MessageCache() {
	this->destroy();
}

void MessageCache::init() {
	for (int i = 0; i < NUM_BUCKETS_IN_CACHE; ++i) {
		MessageBucket* messageBucket = new MessageBucket();
		this->messageBuckets.push_back(messageBucket);
	}
}

void MessageCache::PushMessageForReceipientId(MessageChunk messageChunk, ObjectIdType receipientId) {
	int targetBucketNumber = (int)receipientId % NUM_BUCKETS_IN_CACHE;
	this->messageBuckets[targetBucketNumber]->PushBack(messageChunk);
	if (messageChunk->GetMessageType() == MESSAGE_TYPE_FRAME_EVENT) {
		FRAMEWORK->GetLogger()->dbglog("\nPush back msg type frame event for recepient: %d, count: %d", receipientId, this->messageBuckets[targetBucketNumber]->GetCount());
	}

	this->objectIdsWithPendingMessages[receipientId] = true;
}

MessageChunk MessageCache::PopMessageForReceipientId(ObjectIdType receipientId, bool& returnValueIsValid) {
	int targetBucketNumber = (int)receipientId % NUM_BUCKETS_IN_CACHE;
	bool poppedMessageIsValid = false;
	MessageChunk messageChunk = this->messageBuckets[targetBucketNumber]->PopMessageForReceipientId(receipientId, poppedMessageIsValid);
	if (!poppedMessageIsValid) {
		if (this->objectIdsWithPendingMessages.find(receipientId) != this->objectIdsWithPendingMessages.end()) {
			this->objectIdsWithPendingMessages[receipientId] = false;
		}
	}
	returnValueIsValid = poppedMessageIsValid;
	return messageChunk;
}

void MessageCache::ClearMessagesForReceipientId(ObjectIdType receipientId) {
	int targetBucketNumber = (int)receipientId % NUM_BUCKETS_IN_CACHE;
	MessageChunk messageChunk;
	bool poppedMessageIsValid = false;
	do {
		messageChunk = this->messageBuckets[targetBucketNumber]->PopMessageForReceipientId(receipientId, poppedMessageIsValid);
	} while (poppedMessageIsValid);

	if (this->objectIdsWithPendingMessages.find(receipientId) != this->objectIdsWithPendingMessages.end()) {
		this->objectIdsWithPendingMessages[receipientId] = false;
	}
}

void MessageCache::destroy() {
	for (unsigned int i = 0; i < this->messageBuckets.size(); ++i) {
		delete this->messageBuckets[i];
	}
	this->messageBuckets.clear();
}

////////////////////////////////////////////////////////////////////////////////

MessageBucket::MessageBucket() {
}

void MessageBucket::PushBack(MessageChunk messageChunk) {
	this->messageList.push_back(messageChunk);
}

MessageChunk MessageBucket::PopMessageForReceipientId(ObjectIdType receipientId, bool& returnValueIsValid) {
	MessageChunk returnMessage;
	std::list<MessageChunk>::iterator foundMessageIt = std::find_if(this->messageList.begin(), this->messageList.end(),
									   [receipientId] (MessageChunk& messageChunk) {
									   	   return (messageChunk->getReceiverId() == receipientId);
									   });
	if (foundMessageIt != this->messageList.end()) {
		returnMessage = (*foundMessageIt);
		this->messageList.erase(foundMessageIt);
		returnValueIsValid = true;
	} else {
		returnValueIsValid = false;
	}
	return returnMessage;
}

int MessageBucket::GetCount() {
	return this->messageList.size();
}

MessageBucket::~MessageBucket() {
	// TODO [Implement] Implement cleanup of message bucket
}
