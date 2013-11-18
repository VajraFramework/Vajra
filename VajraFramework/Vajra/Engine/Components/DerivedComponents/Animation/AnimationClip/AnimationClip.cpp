#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/AnimationKeyFrame.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Core/Engine.h"

AnimationClip::AnimationClip() {
	this->init();
}

AnimationClip::~AnimationClip() {
	this->destroy();
}

void AnimationClip::Play() {
	// FRAMEWORK->GetLogger()->dbglog("\nPlaying AnimationClip %s", this->clipName.c_str());
	this->isPlaying = true;
}

void AnimationClip::Pause() {
	// FRAMEWORK->GetLogger()->dbglog("\nPausing AnimationClip %s", this->clipName.c_str());
	this->isPlaying = false;
}

void AnimationClip::Resume() {
	// FRAMEWORK->GetLogger()->dbglog("\nResuming AnimationClip %s", this->clipName.c_str());
	this->isPlaying = true;
}

void AnimationClip::Stop() {
	// FRAMEWORK->GetLogger()->dbglog("\nStopping AnimationClip %s", this->clipName.c_str());

	this->isPlaying = false;
	if (this->getNumKeyFrames() > 0) {
		this->setCurrentKeyFrameIndex(0);
	}
}

void AnimationClip::init() {
	this->isPlaying = false;
	this->isLooping = false;
	this->playbackSpeed = 1.0f;
	this->interpolation = 0.0f;
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

void AnimationClip::destroy() {
}
