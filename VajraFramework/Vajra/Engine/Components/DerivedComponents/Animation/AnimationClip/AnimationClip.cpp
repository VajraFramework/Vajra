#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Core/Engine.h"

AnimationClip::AnimationClip() {
	this->init();
}

AnimationClip::~AnimationClip() {
	this->destroy();
}

// This should be in the derived class
#if 0
void AnimationClip::InitAnimationClip(std::string urlOfAnimationClip) {
	this->clipDataAsset = ENGINE->GetAssetLibrary()->GetAsset<AnimationClipDataAsset>(urlOfAnimationClip);
}
#endif

void AnimationClip::Play() {
	this->isPlaying = true;
}

void AnimationClip::Pause() {
	this->isPlaying = false;
}

void AnimationClip::Resume() {
	this->isPlaying = true;
}

void AnimationClip::Stop() {
	this->isPlaying = false;
}

void AnimationClip::init() {
	this->isPlaying = false;
	this->isLooping = false;
	this->playbackSpeed = 1.0f;
}

void AnimationClip::destroy() {
}
