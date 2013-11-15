#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/RigidAnimationClip/RigidAnimationClip.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Core/Engine.h"

RigidAnimationClip::RigidAnimationClip() {
	this->init();
}

RigidAnimationClip::~RigidAnimationClip() {
	this->destroy();
}

void RigidAnimationClip::InitAnimationClip(std::string urlOfAnimationClip) {
	this->clipDataAsset = ENGINE->GetAssetLibrary()->GetAsset<RigidAnimationClipDataAsset>(urlOfAnimationClip);
}

void RigidAnimationClip::init() {
}

void RigidAnimationClip::destroy() {
}
