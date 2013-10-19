#include "Vajra/Common/Messages/Message.h"

Message::Message() {
	this->init(MESSAGE_TYPE_UNSPECIFIED);
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


MessageData* Message::GetMessageData() const {
	// TODO [Cleanup] Deprecate this in favor of GetTypedData implemented in individual derived message types
	return this->messageData;
}

void Message::SetMessageData(MessageData* messageData_) {
	this->messageData = messageData_;
}


void Message::init(MessageType messageType_) {
	this->messageType = messageType_;
	this->messageData = nullptr;

	this->next = nullptr;
}

void Message::destroy() {
	if (this->messageData != nullptr) {
		delete messageData;
	}
}
