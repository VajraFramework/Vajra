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

void AnimationClip::init() {
	this->isPlaying = false;
	this->isLooping = false;
}

void AnimationClip::destroy() {
}

// This should be in the derived class
#if 0
void AnimationClip::InitAnimationClip(std::string urlOfAnimationClip) {
	this->clipDataAsset = ENGINE->GetAssetLibrary()->GetAsset<AnimationClipDataAsset>(urlOfAnimationClip);
}
#endif

void AnimationClip::Play() {
}

void AnimationClip::Pause() {
}

void AnimationClip::Resume() {
}

void AnimationClip::Stop() {
}
