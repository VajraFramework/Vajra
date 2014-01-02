#ifndef BAKED_SKELETAL_ANIMATION_CLIP_H
#define BAKED_SKELETAL_ANIMATION_CLIP_H

#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/BSAClipDataAsset/BSAClipDataAsset.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"

#include <memory>
#include <string>

// Forward Declarations:
class AnimationKeyFrame;
class BakedSkeletalAnimationKeyFrame;

class BakedSkeletalAnimationClip : public AnimationClip {
public:
	BakedSkeletalAnimationClip(Animation* parentAnimationComponent_);
	virtual ~BakedSkeletalAnimationClip();

	virtual void InitAnimationClip(std::string urlOfAnimationClip);
	virtual void InitAnimationClip(std::string clipName_, std::vector<AnimationKeyFrame*> animationKeyFrames);

protected:
	// @Override
	virtual AnimationKeyFrame* getCurrentKeyFrame() const;
	virtual AnimationKeyFrame* getNextKeyFrame() const;
	virtual AnimationKeyFrame* getCurrentKeyFrameAtInterpolation(float interpolation);
	virtual unsigned int getCurrentKeyFrameIndex() const;
	virtual void setCurrentKeyFrameIndex(unsigned int newIndex);
	virtual unsigned int getNumKeyFrames() const;

	// @Override
	virtual void reset();
	virtual void bind();

private:
	void init();
	void destroy();

	unsigned int currentKeyFrameIndex;
	std::shared_ptr<BakedSkeletalAnimationClipDataAsset> clipDataAsset;
};

#endif // BAKED_SKELETAL_ANIMATION_CLIP_H
