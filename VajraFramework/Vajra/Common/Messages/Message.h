#ifndef MESSAGE_H
#define MESSAGE_H

#include "Vajra/Common/Messages/Declarations.h"
#include "Vajra/Utilities/CommonDeclarations.h"

class Message {
public:
	Message();
	Message(MessageType messagetype_);
	virtual ~Message();

	inline MessageType GetMessageType() { return this->messageType; }
	inline ObjectIdType GetSenderId() { return this->senderId; }
	void* GetRawData();

	void SetMessageType(MessageType messageType_);
	inline void SetData(void* data_);

private:
	void init(MessageType messageType_);
	void destroy();

	inline ObjectIdType getReceiverId() { return this->receiverId; }
	void setSenderId(ObjectIdType senderId_);
	void setReceiverId(ObjectIdType receiverId_);

	MessageType messageType;

	ObjectIdType senderId;
	ObjectIdType receiverId;
	void* data;

	Message* next;

	friend class MessageHub;
	friend class MessageBucket;
};

#endif // MESSAGE_H
