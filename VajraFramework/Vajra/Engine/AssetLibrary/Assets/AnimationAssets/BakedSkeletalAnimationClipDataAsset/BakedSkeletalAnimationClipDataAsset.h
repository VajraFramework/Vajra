#ifndef RIGID_ANIMATION_CLIP_DATA_ASSET_H
#define RIGID_ANIMATION_CLIP_DATA_ASSET_H

#include "Vajra/Engine/AssetLibrary/Asset.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"

#include "Libraries/glm/glm.hpp"

#include <memory>
#include <string>
#include <vector>

// Forward Declarations:
class BakedSkeletalAnimationKeyFrame;

class BakedSkeletalAnimationClipDataAsset : public AnimationClipDataAsset {
public:
	BakedSkeletalAnimationClipDataAsset();
	BakedSkeletalAnimationClipDataAsset(std::string urlOfAnimation);
	~BakedSkeletalAnimationClipDataAsset();

	// @Override
	virtual AssetType GetAssetType();
	// @Override
	virtual void LoadAsset();

	virtual void AddKeyFrame(AnimationKeyFrame* newKeyFrame);

protected:
	virtual unsigned int getNumKeyFrames() const { return this->animationKeyFrames.size(); }
	virtual AnimationKeyFrame* getKeyFrameAtIndex(unsigned int index) const;

private:
	void init();
	void destroy();

	static AssetType assetType;
	std::vector<BakedSkeletalAnimationKeyFrame*> animationKeyFrames;
};

#endif // RIGID_ANIMATION_CLIP_DATA_ASSET_H
