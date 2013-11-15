#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/RigidAnimationClipDataAsset/RigidAnimationClipDataAsset.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
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
	VERIFY(0, "Not implemented. ");
}

void RigidAnimationClipDataAsset::init() {
}

void RigidAnimationClipDataAsset::destroy() {
}
