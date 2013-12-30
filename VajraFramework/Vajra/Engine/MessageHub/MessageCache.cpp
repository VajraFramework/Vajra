#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/MessageHub/MessageCache.h"
#include "Vajra/Utilities/Utilities.h"

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

void MessageCache::PushMessageForReceipientId(Message* message, ObjectIdType receipientId) {
	int targetBucketNumber = (int)receipientId % NUM_BUCKETS_IN_CACHE;
	this->messageBuckets[targetBucketNumber]->PushBack(message);

	this->objectIdsWithPendingMessages[receipientId] = true;
}

Message* MessageCache::PopMessageForReceipientId(ObjectIdType receipientId) {
	int targetBucketNumber = (int)receipientId % NUM_BUCKETS_IN_CACHE;
	Message* message = this->messageBuckets[targetBucketNumber]->PopMessageForReceipientId(receipientId);
	if (message == nullptr) {
		if (this->objectIdsWithPendingMessages.find(receipientId) != this->objectIdsWithPendingMessages.end()) {
			this->objectIdsWithPendingMessages[receipientId] = false;
		}
	}
	return message;
}

void MessageCache::ClearMessagesForReceipientId(ObjectIdType receipientId) {
	int targetBucketNumber = (int)receipientId % NUM_BUCKETS_IN_CACHE;
	Message* message = nullptr;
	do {
		message = this->messageBuckets[targetBucketNumber]->PopMessageForReceipientId(receipientId);
		if (message != nullptr) {
			delete (message);
		}
	} while (message != nullptr);

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

void MessageBucket::PushBack(Message* message) {
	this->messageList.push_back(message);
}

Message* MessageBucket::PopMessageForReceipientId(ObjectIdType receipientId) {
	Message* returnMessage = nullptr;
	std::list<Message*>::iterator foundMessageIt = std::find_if(this->messageList.begin(), this->messageList.end(),
									   [receipientId] (const Message* message) {
									   	   return (message->getReceiverId() == receipientId);
									   });
	if (foundMessageIt != this->messageList.end()) {
		returnMessage = (*foundMessageIt);
		this->messageList.erase(foundMessageIt);
	}
	return returnMessage;
}

int MessageBucket::GetCount() {
	return this->messageList.size();
}

MessageBucket::~MessageBucket() {
	// TODO [Implement] Implement cleanup of message bucket
}
