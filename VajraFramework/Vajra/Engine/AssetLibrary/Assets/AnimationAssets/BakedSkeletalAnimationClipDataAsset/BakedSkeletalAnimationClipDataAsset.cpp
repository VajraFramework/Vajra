#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/BakedSkeletalAnimationKeyframe/BakedSkeletalAnimationKeyframe.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/BakedSkeletalAnimationClipDataAsset/BakedSkeletalAnimationClipDataAsset.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/ModelLoader/BakedSkeletalAnimationLoader.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

AssetType BakedSkeletalAnimationClipDataAsset::assetType = ASSET_TYPE_ANIMATION_DATA_BAKED_SKELETAL;

BakedSkeletalAnimationClipDataAsset::BakedSkeletalAnimationClipDataAsset() : AnimationClipDataAsset() {
    this->init();
}

BakedSkeletalAnimationClipDataAsset::BakedSkeletalAnimationClipDataAsset(std::string urlOfAnimation) : AnimationClipDataAsset(urlOfAnimation) {
	this->init();
}

BakedSkeletalAnimationClipDataAsset::~BakedSkeletalAnimationClipDataAsset() {
    this->destroy();
}

AssetType BakedSkeletalAnimationClipDataAsset::GetAssetType() {
	return BakedSkeletalAnimationClipDataAsset::assetType;
}

void BakedSkeletalAnimationClipDataAsset::LoadAsset() {

	// Split this url into file path and clip name
	std::string clipUrl = this->GetFilePathToAnimationFile();
	int positionToSplitOn = clipUrl.find_first_of('#');
	std::string clipFilePath = clipUrl.substr(0, positionToSplitOn);
	std::string clipName = clipUrl.substr(positionToSplitOn + 1);

	VERIFY(clipFilePath != "" && clipName != "", "Malformed clip url: %s, %s", clipUrl.c_str());

	AnimationLoader::LoadBakedSkeletalAnimationFromAnimclipsFile(clipFilePath.c_str(),
														 clipName,
														 &(this->animationKeyFrames));
	this->setName(clipName);
}

void BakedSkeletalAnimationClipDataAsset::AddKeyFrame(AnimationKeyFrame* newKeyFrame) {
	// TODO [Implement] Figure out how to ensure type safety here:
	this->animationKeyFrames.push_back(dynamic_cast<BakedSkeletalAnimationKeyFrame*>(newKeyFrame));
}

AnimationKeyFrame* BakedSkeletalAnimationClipDataAsset::getKeyFrameAtIndex(unsigned int index) const {
	VERIFY(index < this->animationKeyFrames.size(), "Index %d is valid", index);

	return this->animationKeyFrames[index];
}

void BakedSkeletalAnimationClipDataAsset::init() {
}

void BakedSkeletalAnimationClipDataAsset::destroy() {
	if (!this->animationKeyFrames.empty()) {
		for (BakedSkeletalAnimationKeyFrame* keyFrame : this->animationKeyFrames) {
			delete keyFrame;
		}
		this->animationKeyFrames.clear();
	}
}
