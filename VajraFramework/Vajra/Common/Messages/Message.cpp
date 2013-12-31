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

void Message::init(MessageType messageType_) {
	this->messageType = messageType_;
}

void Message::destroy() {
}
