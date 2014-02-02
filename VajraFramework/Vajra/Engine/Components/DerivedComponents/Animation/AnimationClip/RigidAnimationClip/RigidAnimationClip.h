#ifndef RIGID_ANIMATION_CLIP_H
#define RIGID_ANIMATION_CLIP_H

#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/RigidAnimationClipDataAsset/RigidAnimationClipDataAsset.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"

#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/quaternion.hpp"
#include "Libraries/glm/gtx/quaternion.hpp"

#include <memory>
#include <string>

// Forward Declarations:
class AnimationKeyFrame;
class RigidAnimationKeyFrame;

class RigidAnimationClip : public AnimationClip {
public:
	RigidAnimationClip(Animation* parentAnimationComponent_);
	virtual ~RigidAnimationClip();

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

private:
	void init(Transform* initialTransform);
	void destroy();

	unsigned int currentKeyFrameIndex;
	std::shared_ptr<RigidAnimationClipDataAsset> clipDataAsset;

	// Save the original position, orientation, and scale of the game object so that we can apply the animation as an offset over it
	glm::vec3 initialPosition;
	glm::quat initialOrientation;
	glm::vec3 initialScale;

	RigidAnimationKeyFrame* tempKeyFrame;
};

#endif // RIGID_ANIMATION_CLIP_H
