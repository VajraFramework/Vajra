#ifndef RIGID_ANIMATION_CLIP_H
#define RIGID_ANIMATION_CLIP_H

#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/RigidAnimationClipDataAsset/RigidAnimationClipDataAsset.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"

#include <memory>
#include <string>

class RigidAnimationClip : public AnimationClip {
public:
	RigidAnimationClip();
	virtual ~RigidAnimationClip();

	virtual void InitAnimationClip(std::string urlOfAnimationClip);

private:
	void init();
	void destroy();

	std::shared_ptr<RigidAnimationClipDataAsset> clipDataAsset;
};

#endif // RIGID_ANIMATION_CLIP_H
