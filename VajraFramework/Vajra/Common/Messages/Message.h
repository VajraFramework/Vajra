#ifndef MESSAGE_H
#define MESSAGE_H

#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1IV3FV.h"
#include "Vajra/Common/Messages/Declarations.h"
#include "Vajra/Common/Messages/MessageData.h"
#include "Vajra/Utilities/CommonDeclarations.h"

typedef unsigned int MessageType;

class Message {
public:
	Message();
	Message(MessageType messagetype_);
	virtual ~Message();

	inline MessageType GetMessageType() const { return this->messageType; }
	inline ObjectIdType GetSenderId() const { return this->senderId; }

	void SetMessageType(MessageType messageType_);

	// TODO [Hack] Consider storing a MessageData type here, but as a ManagedChunk from its own pool
	MessageData1S1IV3FV messageData;

private:
	void init(MessageType messageType_);
	void destroy();

	void setSenderId(ObjectIdType senderId_);

	MessageType messageType;

	ObjectIdType senderId;

	// TODO [Implement] Add a field for the time at which this message was sent here

	friend class MessageHub;
	friend class MessageBucket;
};

#endif // MESSAGE_H
