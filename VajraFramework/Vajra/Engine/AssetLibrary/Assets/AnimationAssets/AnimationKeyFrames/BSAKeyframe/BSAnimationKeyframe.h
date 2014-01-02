#ifndef BAKED_SKELETAL_ANIMATION_KEY_FRAME_H
#define BAKED_SKELETAL_ANIMATION_KEY_FRAME_H

#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/AnimationKeyFrame.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Declarations.h"
#include "Vajra/Utilities/MathUtilities.h"

#include "Libraries/glm/glm.hpp"

// Forward Declarations:
class Transform;

class BakedSkeletalAnimationKeyFrame : public AnimationKeyFrame {
public:
	BakedSkeletalAnimationKeyFrame();
	virtual ~BakedSkeletalAnimationKeyFrame();

	// @Override
	virtual void ApplyKeyFrameToTransform(Transform* transform) const;

	void SetBakedBoneMatrixForBone(glm::mat4 bakedBoneMatrix, unsigned int boneId);

private:
	void init();
	void destroy();

	glm::mat4 bakedBoneMatrices[MAX_BONES];;

	friend class BakedSkeletalAnimationClip;

};


#endif // BAKED_SKELETAL_ANIMATION_KEY_FRAME_H

