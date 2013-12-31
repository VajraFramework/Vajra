#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1I3FV.h"
#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/AnimationKeyFrame.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/Animation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/Tween/Tween.h"

AnimationClip::AnimationClip(Animation* parentAnimationComponent_) {
	this->init(parentAnimationComponent_);
}

AnimationClip::~AnimationClip() {
	this->destroy();
}

void AnimationClip::raiseEvent(MessageChunk messageChunk, int gameObjectId, std::string eventClipName) {
	MessageData1S1I3FV* messageData = new MessageData1S1I3FV();
	messageData->i = gameObjectId;
	messageData->s = eventClipName;
	messageChunk->SetMessageData(messageData);

	ObjectIdType parentObjectId = this->parentAnimationComponent->GetGameObject()->GetId();
	ENGINE->GetMessageHub()->SendPointcastMessage(messageChunk, parentObjectId, parentObjectId);
	if (this->isTween) {
		ENGINE->GetMessageHub()->SendPointcastMessage(messageChunk, ENGINE->GetTween()->GetId(), parentObjectId);
	}
}

void AnimationClip::Play() {
	this->isPlaying = true;
	// FRAMEWORK->GetLogger()->dbglog("\nPlaying AnimationClip %s", this->clipName.c_str());
	MessageChunk messageChunk = ENGINE->GetMessageHub()->GetOneFreeMessage();
	messageChunk->SetMessageType(MESSAGE_TYPE_ANIMATION_BEGAN_EVENT);
	this->raiseEvent(messageChunk, this->parentAnimationComponent->GetGameObject()->GetId(), this->GetName());
}

void AnimationClip::Pause() {
	this->isPlaying = false;
	// FRAMEWORK->GetLogger()->dbglog("\nPausing AnimationClip %s", this->clipName.c_str());
	MessageChunk messageChunk = ENGINE->GetMessageHub()->GetOneFreeMessage();
	messageChunk->SetMessageType(MESSAGE_TYPE_ANIMATION_PAUSED_EVENT);
	this->raiseEvent(messageChunk, this->parentAnimationComponent->GetGameObject()->GetId(), this->GetName());
}

void AnimationClip::Resume() {
	this->isPlaying = true;
	// FRAMEWORK->GetLogger()->dbglog("\nResuming AnimationClip %s", this->clipName.c_str());
	MessageChunk messageChunk = ENGINE->GetMessageHub()->GetOneFreeMessage();
	messageChunk->SetMessageType(MESSAGE_TYPE_ANIMATION_RESUMED_EVENT);
	this->raiseEvent(messageChunk, this->parentAnimationComponent->GetGameObject()->GetId(), this->GetName());
}

void AnimationClip::Stop() {
	this->isPlaying = false;
	this->reset();
	if (this->getNumKeyFrames() > 0) {
		this->setCurrentKeyFrameIndex(0);
	}
	// FRAMEWORK->GetLogger()->dbglog("\nStopping AnimationClip %s", this->clipName.c_str());

	MessageChunk messageChunk = ENGINE->GetMessageHub()->GetOneFreeMessage();
	messageChunk->SetMessageType(MESSAGE_TYPE_ANIMATION_ENDED_EVENT);
	this->raiseEvent(messageChunk, this->parentAnimationComponent->GetGameObject()->GetId(), this->GetName());
}

void AnimationClip::step(double deltaTime) {
	ASSERT(this->isPlaying, "Animation clip is playing");

	AnimationKeyFrame* currentKeyFrame = this->getCurrentKeyFrame();
	AnimationKeyFrame* nextKeyFrame = this->getNextKeyFrame();
	if (nextKeyFrame != nullptr) {
		double deltaBetweenFrames = nextKeyFrame->GetTime() - currentKeyFrame->GetTime();
		this->interpolation = this->interpolation + deltaTime / deltaBetweenFrames * this->playbackSpeed;
		if (this->interpolation > 1.0f) {
			this->interpolation = 0.0f;
			this->setCurrentKeyFrameIndex(this->getCurrentKeyFrameIndex() + 1);
		}

	} else {
		this->Stop();
		if (this->isLooping) {
			this->Play();
		}
	}
}

void AnimationClip::apply(Transform* transform) {
	AnimationKeyFrame* keyframe = this->getCurrentKeyFrameAtInterpolation(this->interpolation);
	if (keyframe != nullptr) {
		keyframe->ApplyKeyFrameToTransform(transform);
	}
}

void AnimationClip::init(Animation* parentAnimationComponent_) {
	this->parentAnimationComponent = parentAnimationComponent_;
	this->isPlaying = false;
	this->isLooping = false;
	this->isTween   = false;
	this->playbackSpeed = 1.0f;
	this->interpolation = 0.0f;
}


void AnimationClip::destroy() {
}
