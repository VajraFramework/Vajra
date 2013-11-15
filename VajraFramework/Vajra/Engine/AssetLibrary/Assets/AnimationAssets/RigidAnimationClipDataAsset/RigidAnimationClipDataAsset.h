#ifndef RIGID_ANIMATION_CLIP_DATA_ASSET_H
#define RIGID_ANIMATION_CLIP_DATA_ASSET_H

#include "Vajra/Engine/AssetLibrary/Asset.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"

#include "Libraries/glm/glm.hpp"

#include <memory>
#include <string>
#include <vector>

class RigidAnimationClipDataAsset : public AnimationClipDataAsset {
public:
	RigidAnimationClipDataAsset();
	RigidAnimationClipDataAsset(std::string urlOfAnimation);
	~RigidAnimationClipDataAsset();

	// @Override
	virtual AssetType GetAssetType();
	// @Override
	virtual void LoadAsset();

private:
	void init();
	void destroy();

	static AssetType assetType;
};

#endif // RIGID_ANIMATION_CLIP_DATA_ASSET_H
