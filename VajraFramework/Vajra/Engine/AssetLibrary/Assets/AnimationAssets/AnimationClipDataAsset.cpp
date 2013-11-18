#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/AnimationKeyFrame.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

AssetType AnimationClipDataAsset::assetType = ASSET_TYPE_ANIMATION_DATA;

AnimationClipDataAsset::AnimationClipDataAsset() : Asset() {
    this->init();
}

AnimationClipDataAsset::AnimationClipDataAsset(std::string urlOfAnimation) : Asset(urlOfAnimation) {
	this->init();
}

AnimationClipDataAsset::~AnimationClipDataAsset() {
    this->destroy();
}

AssetType AnimationClipDataAsset::GetAssetType() {
	return AnimationClipDataAsset::assetType;
}

std::string AnimationClipDataAsset::GetFilePathToAnimationFile() {
	return this->GetUrl();
}

unsigned int AnimationClipDataAsset::GetNumKeyFrames() const {
	return this->getNumKeyFrames();
}

AnimationKeyFrame* AnimationClipDataAsset::GetKeyFrameAtIndex(unsigned int index) const {
	return this->getKeyFrameAtIndex(index);
}

void AnimationClipDataAsset::setName(std::string name_) {
	this->name = name_;
}

void AnimationClipDataAsset::init() {
}

void AnimationClipDataAsset::destroy() {
}
