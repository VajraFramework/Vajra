#ifndef ANIMATION_CLIP_DATA_ASSET_H
#define ANIMATION_CLIP_DATA_ASSET_H

#include "Vajra/Engine/AssetLibrary/Asset.h"

#include "Libraries/glm/glm.hpp"

#include <memory>
#include <string>
#include <vector>

class AnimationClipDataAsset : public Asset {
public:
	AnimationClipDataAsset();
	AnimationClipDataAsset(std::string urlOfAnimation);
	virtual ~AnimationClipDataAsset();

	// @Override
	virtual AssetType GetAssetType();
	// @Override
	virtual void LoadAsset() = 0;

	std::string GetFilePathToAnimationFile();

private:
	void init();
	void destroy();

	static AssetType assetType;
};

#endif // ANIMATION_CLIP_DATA_ASSET_H
