#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/RigidAnimation/RigidAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/RigidAnimationClip/RigidAnimationClip.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"


unsigned int RigidAnimation::componentTypeId = COMPONENT_TYPE_ID_RIGID_ANIMATION;

RigidAnimation::RigidAnimation() : Animation(), Component() {
	this->init();
}

RigidAnimation::RigidAnimation(Object* object_) : Animation((GameObject*)object_), Component(object_) {
	this->init();
}

RigidAnimation::~RigidAnimation() {
	this->destroy();
}

AnimationClip* RigidAnimation::AddAnimationClip(std::string urlOfAnimationClip) {
	RigidAnimationClip* newRigidAnimationClip = new RigidAnimationClip(this);
	newRigidAnimationClip->InitAnimationClip(urlOfAnimationClip);

	return this->AddAnimationClip(newRigidAnimationClip);
}

AnimationClip* RigidAnimation::AddAnimationClip(AnimationClip* animationClip) {
	return this->addAnimationClip(animationClip, true);
}

void RigidAnimation::HandleMessage(MessageChunk messageChunk) {
	if (this->handleMessage(messageChunk) == NOT_HANDLED) {

		switch (messageChunk->GetMessageType()) {

		default:
			FRAMEWORK->GetLogger()->dbglog("\nRigidAnimation got unnecessary msg of type %d", messageChunk->GetMessageType());
			break;
		}
	}
}


void RigidAnimation::Bind() {
	// Nothing to do here
}

void RigidAnimation::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	// TODO [Implement] Figure out if its better to add/remove subscription dynamically on play/pause/remove
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void RigidAnimation::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

