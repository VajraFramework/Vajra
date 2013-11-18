#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/Animation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

unsigned int Animation::componentTypeId = COMPONENT_TYPE_ID_ANIMATION;

Animation::Animation() : Component() {
	this->init();
}

Animation::Animation(Object* object_) : Component(object_) {
	this->init();
}

Animation::~Animation() {
	this->destroy();
}

void Animation::HandleMessage(Message* message) {
	switch (message->GetMessageType()) {

	case MESSAGE_TYPE_FRAME_EVENT:
		if (this->IsPlaying()) {
			this->step(ENGINE->GetTimer()->GetDeltaFrameTime());
			this->apply();
		}
		break;

	default:
		break;
	}
}

void Animation::step(double deltaTime) {
	this->currentAnimationClip->step(deltaTime);
}

void Animation::apply() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	this->currentAnimationClip->apply(gameObject->GetTransform());
}

AnimationClip* Animation::addAnimationClip(AnimationClip* animationClip, bool takeOwnershipOfMemory) {
	if (this->animationClips.find(animationClip->GetName()) == this->animationClips.end()) {
		this->animationClips[animationClip->GetName()] = animationClip;
		return animationClip;

	} else {
		FRAMEWORK->GetLogger()->dbglog("\nTried to add duplicate animation clip at url: %s to Object of id: %d", animationClip->GetName().c_str(), this->GetObject()->GetId());
		if (takeOwnershipOfMemory) {
			delete animationClip;
		}
		return this->animationClips[animationClip->GetName()];
	}
}

void Animation::DeleteAnimationClip(std::string animationClipName) {
	if (!this->animationClips.erase(animationClipName)) {
		FRAMEWORK->GetLogger()->dbglog("\nTried to delete unadded animation clip at url: %s from Object of id: %d", animationClipName.c_str(), this->GetObject()->GetId());
	}
}

AnimationClip* Animation::GetAnimationClip(std::string animationClipName) {
	auto animationClipIt = this->animationClips.find(animationClipName);
	if (animationClipIt != this->animationClips.end()) {
		return animationClipIt->second;
	}
	return nullptr;
}

void Animation::PlayAnimationClip(std::string animationClipName) {
	auto animationClipIt = this->animationClips.find(animationClipName);
	if (animationClipIt != this->animationClips.end()) {
		this->playAnimationClip_internal(animationClipIt->second);
	} else {
		FRAMEWORK->GetLogger()->dbglog("\nNo such clip to play: %s", animationClipName.c_str());
	}
}

void Animation::PlayAnimationClip() {
	if (this->currentAnimationClip != nullptr) {
		this->playAnimationClip_internal(this->currentAnimationClip);
	}
}

void Animation::PauseAnimationClip() {
	if (this->currentAnimationClip != nullptr) {
		this->currentAnimationClip->Pause();
	}
}

void Animation::ResumeAnimationClip() {
	if (this->currentAnimationClip != nullptr) {
		this->currentAnimationClip->Resume();
	}
}

void Animation::StopAnimationClip() {
	if (this->currentAnimationClip != nullptr) {
		this->currentAnimationClip->Stop();
		this->currentAnimationClip = nullptr;
	}
}

bool Animation::IsPlaying() {
	if (this->currentAnimationClip != nullptr && this->currentAnimationClip->IsPlaying()) {
		return true;
	}
	return false;
}

bool Animation::IsPlaying(std::string animationClipName) {
	if (this->IsPlaying()) {
		if (this->currentAnimationClip != nullptr && this->currentAnimationClip->GetName() == animationClipName) {
			return true;
		}
	}
	return false;
}

void Animation::playAnimationClip_internal(AnimationClip* animationClip) {
	VERIFY(animationClip != nullptr, "animationClip not null");

	if (this->currentAnimationClip != nullptr) {
		this->currentAnimationClip->Stop();
	}
	animationClip->Play();

	this->currentAnimationClip = animationClip;
}

void Animation::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->currentAnimationClip = nullptr;

	// TODO [Implement] Figure out if its better to add/remove subscription dynamically on play/pause/remove
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void Animation::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

