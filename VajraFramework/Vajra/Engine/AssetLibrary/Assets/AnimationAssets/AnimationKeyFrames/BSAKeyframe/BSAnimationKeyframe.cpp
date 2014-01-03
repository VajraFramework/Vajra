#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/BSAKeyframe/BSAnimationKeyframe.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/MathUtilities.h"

BakedSkeletalAnimationKeyFrame::BakedSkeletalAnimationKeyFrame() {
	this->init();
}

BakedSkeletalAnimationKeyFrame::~BakedSkeletalAnimationKeyFrame() {
	this->destroy();
}

void BakedSkeletalAnimationKeyFrame::ApplyKeyFrameToTransform(Transform* /* transform */) const {
	// Nothing to do here.
}

void BakedSkeletalAnimationKeyFrame::SetBakedBoneMatrixForBone(glm::mat4 bakedBoneMatrix, unsigned int boneId) {
	VERIFY(boneId < MAX_BONES, "boneId (%u) less than max bones (%u)", boneId, MAX_BONES);
	this->bakedBoneMatrices[boneId] = bakedBoneMatrix;
}

void BakedSkeletalAnimationKeyFrame::init() {
	for (int i = 0; i < MAX_BONES; ++i) {
		this->bakedBoneMatrices[i] = IDENTITY_MATRIX;
	}
}

void BakedSkeletalAnimationKeyFrame::destroy() {
}

