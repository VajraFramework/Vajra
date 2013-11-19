#ifndef RIGID_ANIMATION_CLIP_DATA_ASSET_H
#define RIGID_ANIMATION_CLIP_DATA_ASSET_H

#include "Vajra/Engine/AssetLibrary/Asset.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"

#include "Libraries/glm/glm.hpp"

#include <memory>
#include <string>
#include <vector>

// Forward Declarations:
class RigidAnimationKeyFrame;

class RigidAnimationClipDataAsset : public AnimationClipDataAsset {
public:
	RigidAnimationClipDataAsset();
	RigidAnimationClipDataAsset(std::string urlOfAnimation);
	~RigidAnimationClipDataAsset();

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
	std::vector<RigidAnimationKeyFrame*> animationKeyFrames;
};

#endif // RIGID_ANIMATION_CLIP_DATA_ASSET_H
