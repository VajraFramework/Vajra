#ifndef MESSAGE_HUB_H
#define MESSAGE_HUB_H

#include "Vajra/Engine/MessageHub/MessageCache.h"
#include "Vajra/Utilities/CommonDeclarations.h"

// Forward Declarations:
class Message;
class MessageCache;

class MessageHub {
public:
	~MessageHub();

	void SendPointcastMessage(Message* message, ObjectIdType receiverId, ObjectIdType senderId = OBJECT_ID_INVALID);
	void SendMulticastMessage(Message* message, ObjectIdType senderId = OBJECT_ID_INVALID);

	Message* RetrieveNextMessage(ObjectIdType id);

	void DrainMessages();

private:
	MessageHub();
	void init();
	void destroy();

	void drainMessageCache_internal();

	MessageCache frontMessageCache;
	MessageCache backMessageCache;

	MessageCache* currentlyAcceptingMessageCache;
	MessageCache* currentlyDrainingMessageCache;

	friend class Engine;
};

#endif // MESSAGE_HUB_H
