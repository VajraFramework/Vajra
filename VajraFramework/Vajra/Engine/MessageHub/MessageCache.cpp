#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/MessageHub/MessageCache.h"
#include "Vajra/Utilities/Utilities.h"

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
	this->head = nullptr;
	this->count = 0;
}

void MessageBucket::PushBack(Message* message) {
	if (this->head == nullptr) {
		this->head = message;

	} else {
		Message* current = this->head;
		while (current->next != nullptr) {
			current = current->next;
		}
		current->next = message;
	}
	ASSERT(message->next == nullptr, "Single message given to MessageHub, not linked list [unsupported yet]");

	this->count++;
}

Message* MessageBucket::PopMessageForReceipientId(ObjectIdType receipientId) {
	Message* current = this->head;
	Message* prev = nullptr;

	while (current != nullptr) {
		if (current->getReceiverId() == receipientId) {
			// Done, found.
			break;
		}
		prev = current;
		current = current->next;
	}

	if (current != nullptr) {
		if (current == this->head) {
			this->head = current->next;
		} else if (prev != nullptr) {
			prev->next = current->next;
		}
		this->count--;
	}

	return current;
}

int MessageBucket::GetCount() {
	return this->count;
}

MessageBucket::~MessageBucket() {
	Message* current = this->head;
	Message* next = nullptr;
	while (current != nullptr) {
		next = current->next;
		delete (current);
		current = next;
	}
	this->count = 0;
}
