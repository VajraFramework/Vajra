#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/Ui/UiCallbackComponent/UiCallbackComponent.h"
#include "Vajra/Engine/Ui/UiTouchHandlers/UiTouchHandlers.h"

ComponentIdType UiCallbackComponent::componentTypeId = COMPONENT_TYPE_ID_UI_CALLBACK;

UiCallbackComponent::UiCallbackComponent() : Component() {
	this->init();
}

UiCallbackComponent::UiCallbackComponent(Object* object_) : Component(object_) {
	this->init();
}

UiCallbackComponent::~UiCallbackComponent() {
	this->destroy();
}

void UiCallbackComponent::HandleMessage(MessageChunk messageChunk) {
	this->touchHandlers->HandleMessageCallback(messageChunk);
}

void UiCallbackComponent::SubscribeToMessage(MessageType messageType) {
	this->addSubscriptionToMessageType(messageType, this->GetTypeId(), false);
}

void UiCallbackComponent::init() {
}

void UiCallbackComponent::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
