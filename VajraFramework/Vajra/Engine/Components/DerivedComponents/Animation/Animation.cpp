#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/Animation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

Animation::Animation() {
	ASSERT(0, "Don't use this constructor");
}

Animation::Animation(GameObject* gameObject_) {
	this->init(gameObject_);
}

Animation::~Animation() {
	this->destroy();
}

bool Animation::handleMessage(MessageChunk messageChunk) {
	bool handled = false;

	switch (messageChunk->GetMessageType()) {

	case MESSAGE_TYPE_FRAME_EVENT:
		if (this->IsPlaying()) {
			this->step(ENGINE->GetTimer()->GetDeltaFrameTime());
		}
		if (this->IsPlaying()) {
			this->apply();
		}
		handled = true;
		break;

	default:
		break;
	}

	return handled;
}

void Animation::step(double deltaTime) {
	this->currentAnimationClip->step(deltaTime);
}

void Animation::apply() {
	this->currentAnimationClip->apply(this->gameObject->GetTransform());
}

void Animation::bind() {
	if (this->currentAnimationClip != nullptr) {
		this->currentAnimationClip->bind();
	}
}

AnimationClip* Animation::addAnimationClip(AnimationClip* animationClip, bool takeOwnershipOfMemory) {
	if (this->animationClips.find(animationClip->GetName()) == this->animationClips.end()) {
		this->animationClips[animationClip->GetName()] = animationClip;
		return animationClip;

	} else {
		FRAMEWORK->GetLogger()->dbglog("\nTried to add duplicate animation clip at url: %s, to game object of id: %u", animationClip->GetName().c_str(), this->gameObject->GetId());
		if (takeOwnershipOfMemory) {
			delete animationClip;
		}
		return this->animationClips[animationClip->GetName()];
	}
}

void Animation::DeleteAnimationClip(std::string animationClipName) {
	if (!this->animationClips.erase(animationClipName)) {
		FRAMEWORK->GetLogger()->dbglog("\nTried to delete unadded animation clip at url: %s, from game object of id: %u", animationClipName.c_str(), this->gameObject->GetId());
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

void Animation::init(GameObject* gameObject_) {
	this->currentAnimationClip = nullptr;
	this->gameObject = gameObject_;
}

void Animation::destroy() {
}

