#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/RigidAnimationKeyFrame/RigidAnimationKeyFrame.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/RigidAnimationClipDataAsset/RigidAnimationClipDataAsset.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/ModelLoader/RigidAnimationLoader.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

AssetType RigidAnimationClipDataAsset::assetType = ASSET_TYPE_ANIMATION_DATA_RIGID;

RigidAnimationClipDataAsset::RigidAnimationClipDataAsset() : AnimationClipDataAsset() {
    this->init();
}

RigidAnimationClipDataAsset::RigidAnimationClipDataAsset(std::string urlOfAnimation) : AnimationClipDataAsset(urlOfAnimation) {
	this->init();
}

RigidAnimationClipDataAsset::~RigidAnimationClipDataAsset() {
    this->destroy();
}

AssetType RigidAnimationClipDataAsset::GetAssetType() {
	return RigidAnimationClipDataAsset::assetType;
}

void RigidAnimationClipDataAsset::LoadAsset() {

	// Split this url into file path and clip name
	std::string clipUrl = this->GetFilePathToAnimationFile();
	int positionToSplitOn = clipUrl.find_first_of('#');
	std::string clipFilePath = clipUrl.substr(0, positionToSplitOn);
	std::string clipName = clipUrl.substr(positionToSplitOn + 1);

	VERIFY(clipFilePath != "" && clipName != "", "Malformed clip url: %s, %s", clipUrl.c_str());

	AnimationLoader::LoadRigidAnimationFromAnimclipsFile(clipFilePath.c_str(),
														 clipName,
														 &(this->animationKeyFrames));
	this->setName(clipName);
}

void RigidAnimationClipDataAsset::AddKeyFrame(AnimationKeyFrame* newKeyFrame) {
	// TODO [Implement] Figure out how to ensure type safety here:
	this->animationKeyFrames.push_back(dynamic_cast<RigidAnimationKeyFrame*>(newKeyFrame));
}

AnimationKeyFrame* RigidAnimationClipDataAsset::getKeyFrameAtIndex(unsigned int index) const {
	VERIFY(index < this->animationKeyFrames.size(), "Index %d is valid", index);

	return this->animationKeyFrames[index];
}

void RigidAnimationClipDataAsset::init() {
}

void RigidAnimationClipDataAsset::destroy() {
	if (!this->animationKeyFrames.empty()) {
		for (RigidAnimationKeyFrame* keyFrame : this->animationKeyFrames) {
			delete keyFrame;
		}
		this->animationKeyFrames.clear();
	}
}
