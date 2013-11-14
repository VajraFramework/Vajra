#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/Animation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
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

	default:
		break;
	}
}

// This should be in the derived class
#if 0
void Animation::AddAnimationClip(std::string urlOfAnimationClip) {
	AnimationClip* newAnimationClip = new AnimationClip();
	newAnimationClip->InitAnimationClip(urlOfAnimationClip);

	if (this->animationClips.find(newAnimationClip->GetName()) == this->animationClips.end()) {
		this->animationClips[newAnimationClip->GetName()] = newAnimationClip;
	} else {
		FRAMEWORK->GetLogger()->dbglog("\nTried to add duplicate animation clip at url: %s to Object of id: %d", urlOfAnimationClip.c_str(), this->GetObject()->GetId());
	}
}
#endif

void Animation::PlayAnimationClip(std::string animationClipName) {
	auto animationClipIt = this->animationClips.find(animationClipName);
	if (animationClipIt != this->animationClips.end()) {
		this->playAnimationClip_internal(animationClipIt->second);
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
}

void Animation::destroy() {
}

