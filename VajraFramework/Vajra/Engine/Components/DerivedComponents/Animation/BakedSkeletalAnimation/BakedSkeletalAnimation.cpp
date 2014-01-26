#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/BakedSkeletalAnimation/BakedSkeletalAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/BSAClip/BSAClip.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

unsigned int BakedSkeletalAnimation::componentTypeId = COMPONENT_TYPE_ID_BAKED_SKELETAL_ANIMATION;

BakedSkeletalAnimation::BakedSkeletalAnimation() : Animation(), Component() {
	this->init();
}

BakedSkeletalAnimation::BakedSkeletalAnimation(Object* object_) : Animation((GameObject*)object_), Component(object_) {
	this->init();
}

BakedSkeletalAnimation::~BakedSkeletalAnimation() {
	this->destroy();
}

AnimationClip* BakedSkeletalAnimation::AddAnimationClip(std::string urlOfAnimationClip) {
	BakedSkeletalAnimationClip* newBakedSkeletalAnimationClip = new BakedSkeletalAnimationClip(this);
	newBakedSkeletalAnimationClip->InitAnimationClip(urlOfAnimationClip);

	return this->AddAnimationClip(newBakedSkeletalAnimationClip);
}

AnimationClip* BakedSkeletalAnimation::AddAnimationClip(AnimationClip* animationClip) {
	return this->addAnimationClip(animationClip, true);
}

void BakedSkeletalAnimation::HandleMessage(MessageChunk messageChunk) {
	if (this->handleMessage(messageChunk) == NOT_HANDLED) {

		switch (messageChunk->GetMessageType()) {

		default:
			FRAMEWORK->GetLogger()->dbglog("\nBakedSkeletalAnimation got unnecessary msg of type %d", messageChunk->GetMessageType());
			break;
		}
	}
}


void BakedSkeletalAnimation::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	// TODO [Implement] Figure out if its better to add/remove subscription dynamically on play/pause/remove
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void BakedSkeletalAnimation::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
