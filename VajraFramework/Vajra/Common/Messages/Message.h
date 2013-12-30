#ifndef MESSAGE_H
#define MESSAGE_H

#include "Vajra/Common/Messages/Declarations.h"
#include "Vajra/Common/Messages/MessageData.h"
#include "Vajra/Utilities/CommonDeclarations.h"

class Message {
public:
	Message();
	Message(MessageType messagetype_);
	virtual ~Message();

	inline MessageType GetMessageType() const { return this->messageType; }
	inline ObjectIdType GetSenderId() const { return this->senderId; }
	MessageData* GetMessageData() const;

	void SetMessageType(MessageType messageType_);
	void SetMessageData(MessageData* messageData_);

private:
	void init(MessageType messageType_);
	void destroy();

	inline ObjectIdType getReceiverId() const { return this->receiverId; }
	void setSenderId(ObjectIdType senderId_);
	void setReceiverId(ObjectIdType receiverId_);

	MessageType messageType;

	ObjectIdType senderId;
	ObjectIdType receiverId;
	MessageData* messageData;

	// TODO [Implement] Add a field for the time at which this message was sent here

	friend class MessageHub;
	friend class MessageBucket;
};

#endif // MESSAGE_H
