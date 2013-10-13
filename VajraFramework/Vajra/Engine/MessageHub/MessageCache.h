#ifndef MESSAGE_CACHE_H
#define MESSAGE_CACHE_H

#include "Vajra/Utilities/CommonDeclarations.h"

#include <map>
#include <vector>

// Forward Declarations:
class Message;

class MessageBucket {
public:
	MessageBucket();
	~MessageBucket();

	void PushBack(Message* message);
	Message* PopMessageForReceipientId(ObjectIdType receipientId);

	int GetCount();

private:
	int count;
	Message* head;
};

class MessageCache {
public:
	~MessageCache();

private:
	MessageCache();

	void PushMessageForReceipientId(Message* message, ObjectIdType receipientId);
	Message* PopMessageForReceipientId(ObjectIdType receipientId);
	void ClearMessagesForReceipientId(ObjectIdType receipientId);

	void init();
	void destroy();

	std::vector<MessageBucket*> messageBuckets;
	std::map<ObjectIdType, bool> objectIdsWithPendingMessages;

	friend class MessageHub;
};

#endif // MESSAGE_CACHE_H
