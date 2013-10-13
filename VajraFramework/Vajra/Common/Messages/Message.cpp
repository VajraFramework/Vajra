#include "Vajra/Common/Messages/Message.h"

Message::Message() {
	this->init(MESSAGE_TYPE_BASE);
}

Message::Message(MessageType messagetype_) {
	this->init(messagetype_);
}

Message::~Message() {
}

void Message::SetMessageType(MessageType messageType_) {
	this->messageType = messageType_;
}

void Message::setSenderId(ObjectIdType senderId_) {
	this->senderId = senderId_;
}

void Message::setReceiverId(ObjectIdType receiverId_) {
	this->receiverId = receiverId_;
}


inline void* Message::GetRawData() {
	// TODO [Cleanup] Deprecate this in favor of GetTypedData implemented in individual derived message types
	return this->data;
}

void Message::SetData(void* data_) {
	this->data = data_;
}


void Message::init(MessageType messageType_) {
	this->messageType = messageType_;
	this->data = nullptr;

	this->next = nullptr;
}

void Message::destroy() {
	if (this->data != nullptr) {
		// TODO [Implement] Delete the (void*) data pointer in messages
	}
}
