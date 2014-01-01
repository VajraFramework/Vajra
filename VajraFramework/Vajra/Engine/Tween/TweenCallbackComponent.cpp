#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1I3FV.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Tween/TweenCallbackComponent.h"


unsigned int TweenCallbackComponent::componentTypeId = COMPONENT_TYPE_ID_TWEEN_CALLBACK;

TweenCallbackComponent::TweenCallbackComponent() : Component() {
	this->init();
}

TweenCallbackComponent::TweenCallbackComponent(Object* object_) : Component(object_) {
	this->init();
}

TweenCallbackComponent::~TweenCallbackComponent() {
	this->destroy();
}

void TweenCallbackComponent::HandleMessage(MessageChunk messageChunk) {
	switch (messageChunk->GetMessageType()) {

	case MESSAGE_TYPE_ANIMATION_BEGAN_EVENT:
		break;
	case MESSAGE_TYPE_ANIMATION_PAUSED_EVENT:
		break;
	case MESSAGE_TYPE_ANIMATION_RESUMED_EVENT:
		break;
	case MESSAGE_TYPE_ANIMATION_ENDED_EVENT:
		this->handleCallbacksOnAnimationEnd(messageChunk);
		break;

	default:
		FRAMEWORK->GetLogger()->dbglog("\nTweenCallbackComponent got unnecessary msg of type %d", messageChunk->GetMessageType());
	}
}

void TweenCallbackComponent::handleCallbacksOnAnimationEnd(MessageChunk messageChunk) {
	MessageData1S1I3FV* data = &(messageChunk->messageData);

	ObjectIdType gameObjectId = data->i;

	OnGoingTransformTweenDetails* tweenDetails = ENGINE->GetTween()->getOnGoingTransformTweenDetails(gameObjectId);
	if (tweenDetails != nullptr) {
		if (tweenDetails->callback != 0) {
			tweenDetails->callback(gameObjectId, tweenDetails->tweenClipName);
		}
	} else {
		FRAMEWORK->GetLogger()->dbglog("\nWARNING Received tween animation end event on GameObject %d, but there was no tween going on on it", gameObjectId);
	}
}

void TweenCallbackComponent::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->addSubscriptionToMessageType(MESSAGE_TYPE_ANIMATION_BEGAN_EVENT, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_ANIMATION_PAUSED_EVENT, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_ANIMATION_RESUMED_EVENT, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_ANIMATION_ENDED_EVENT, this->GetTypeId(), true);
}

void TweenCallbackComponent::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
