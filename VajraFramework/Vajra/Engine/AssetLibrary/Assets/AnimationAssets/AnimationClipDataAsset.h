#ifndef ANIMATION_CLIP_DATA_ASSET_H
#define ANIMATION_CLIP_DATA_ASSET_H

#include "Vajra/Engine/AssetLibrary/Asset.h"

#include <memory>
#include <string>
#include <vector>

// Forward Declarations:
class AnimationKeyFrame;

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
	inline std::string GetName() { return this->name; }

	unsigned int GetNumKeyFrames() const;
	AnimationKeyFrame* GetKeyFrameAtIndex(unsigned int index) const;

protected:
	virtual unsigned int getNumKeyFrames() const  = 0;
	virtual AnimationKeyFrame* getKeyFrameAtIndex(unsigned int index) const = 0;
	void setName(std::string name_);

private:
	void init();
	void destroy();

	static AssetType assetType;

	std::string name;

	// This should be in the derived class:
	// std::vector<AnimationKeyFrame*> animationKeyFrames;
};

#endif // ANIMATION_CLIP_DATA_ASSET_H
