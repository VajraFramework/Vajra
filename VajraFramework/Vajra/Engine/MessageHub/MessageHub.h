#ifndef MESSAGE_HUB_H
#define MESSAGE_HUB_H

#include "Vajra/Engine/MessageHub/Declarations.h"
#include "Vajra/Engine/MessageHub/MessageCache.h"
#include "Vajra/Utilities/CommonDeclarations.h"

// Forward Declarations:
class MessageCache;

class MessageHub {
public:
	~MessageHub();

	void SendPointcastMessage(MessageChunk messageChunk, ObjectIdType receiverId, ObjectIdType senderId = OBJECT_ID_INVALID);
	void SendMulticastMessage(MessageChunk messageChunk, ObjectIdType senderId = OBJECT_ID_INVALID);

	void SubscribeToMessageType(MessageType messageType, ObjectIdType subscriberId);
	void UnsubscribeToMessageType(MessageType messageType, ObjectIdType subscriberId);

	MessageChunk RetrieveNextMessage(ObjectIdType id, bool& returnValueIsValid);

	MessageChunk GetOneFreeMessage() { return this->messagePool.GetManagedChunk(); }

private:
	MessageHub();
	void init();
	void destroy();

	void drainMessages();
	void drainMessageCache_internal();

	MessageCache frontMessageCache;
	MessageCache backMessageCache;
	//
	MessageCache* currentlyAcceptingMessageCacheRef;
	MessageCache* currentlyDrainingMessageCacheRef;

	// TODO [Implement] Change this to be a sorted list so that we can binary search it maybe (low priority)
	std::vector<ObjectIdType> subscribersForMessageType[NUM_MESSAGE_TYPES];

	Pool<Message> messagePool;

	friend class Engine;
};

#endif // MESSAGE_HUB_H
