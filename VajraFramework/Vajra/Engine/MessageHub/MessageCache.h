#ifndef MESSAGE_CACHE_H
#define MESSAGE_CACHE_H

#include "Vajra/Utilities/CommonDeclarations.h"

#include <list>
#include <map>
#include <utility>
#include <vector>

// Forward Declarations:
class Message;

class MessageBucket {
public:
	MessageBucket();
	~MessageBucket();

	void PushBack(ObjectIdType receipientId, MessageChunk message);
	// TODO [Hack] Use a custom implementation of boost::optional instead of returnValueIsValid
	MessageChunk PopMessageForReceipientId(ObjectIdType receipientId, bool& returnValueIsValid);

	int GetCount();

private:
	std::list< std::pair<ObjectIdType /* receipientId */, MessageChunk> > messageList;
};

class MessageCache {
public:
	~MessageCache();

private:
	MessageCache();

	void PushMessageForReceipientId(MessageChunk message, ObjectIdType receipientId);
	MessageChunk PopMessageForReceipientId(ObjectIdType receipientId, bool& returnValueIsValid);
	void ClearMessagesForReceipientId(ObjectIdType receipientId);

	void init();
	void destroy();

	std::vector<MessageBucket*> messageBuckets;
	std::map<ObjectIdType, bool> objectIdsWithPendingMessages;

	friend class MessageHub;
};

#endif // MESSAGE_CACHE_H
