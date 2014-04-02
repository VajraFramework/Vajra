#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/BSAKeyframe/BSAnimationKeyframe.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/Animation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/BSAClip/BSAClip.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/MathUtilities.h"

BakedSkeletalAnimationClip::BakedSkeletalAnimationClip(Animation* parentAnimationComponent_) : AnimationClip(parentAnimationComponent_) {
	this->init();
}

BakedSkeletalAnimationClip::~BakedSkeletalAnimationClip() {
	this->destroy();
}

void BakedSkeletalAnimationClip::InitAnimationClip(std::string urlOfAnimationClip) {
	this->clipDataAsset = ENGINE->GetAssetLibrary()->GetAsset<BakedSkeletalAnimationClipDataAsset>(urlOfAnimationClip);
	this->SetName(this->clipDataAsset->GetName());
}

void BakedSkeletalAnimationClip::InitAnimationClip(std::string clipName_, std::vector<AnimationKeyFrame*> animationKeyFramesToAdd) {
	this->clipDataAsset = std::make_shared<BakedSkeletalAnimationClipDataAsset>();
	for (AnimationKeyFrame* keyFrame : animationKeyFramesToAdd) {
		this->clipDataAsset->AddKeyFrame(keyFrame);
	}
	this->SetName(clipName_);
}

AnimationKeyFrame* BakedSkeletalAnimationClip::getCurrentKeyFrame() const {
	if (this->currentKeyFrameIndex < this->getNumKeyFrames()) {
		return this->clipDataAsset->GetKeyFrameAtIndex(this->currentKeyFrameIndex);
	}
	return nullptr;
}

AnimationKeyFrame* BakedSkeletalAnimationClip::getNextKeyFrame() const {
	if ((this->currentKeyFrameIndex + 1) < this->getNumKeyFrames()) {
		return this->clipDataAsset->GetKeyFrameAtIndex(this->currentKeyFrameIndex + 1);
	}
	return nullptr;
}

AnimationKeyFrame* BakedSkeletalAnimationClip::getCurrentKeyFrameAtInterpolation(float /* interpolation */) {
	BakedSkeletalAnimationKeyFrame* currentKeyFrame = (BakedSkeletalAnimationKeyFrame*)this->getCurrentKeyFrame();
	BakedSkeletalAnimationKeyFrame* nextKeyFrame = (BakedSkeletalAnimationKeyFrame*)this->getNextKeyFrame();

	if (currentKeyFrame != nullptr && nextKeyFrame != nullptr) {

	} else {
		// If we couldn't interpolate to next key frame, return current instead (which may or may not be null)
		if (currentKeyFrame != nullptr) {

		}
	}

	return nullptr;
}

void BakedSkeletalAnimationClip::reset() {
	// Nothing to do here
}

unsigned int BakedSkeletalAnimationClip::getCurrentKeyFrameIndex() const {
	return this->currentKeyFrameIndex;
}

void BakedSkeletalAnimationClip::setCurrentKeyFrameIndex(unsigned int index) {
	VERIFY(index < this->getNumKeyFrames(), "Valid key frame index: %d", index);
	this->currentKeyFrameIndex = index;
}

unsigned int BakedSkeletalAnimationClip::getNumKeyFrames() const {
	return this->clipDataAsset->GetNumKeyFrames();
}

void BakedSkeletalAnimationClip::bind() {
	BakedSkeletalAnimationKeyFrame* currentKeyframe = (BakedSkeletalAnimationKeyFrame*)this->getCurrentKeyFrame();
	BakedSkeletalAnimationKeyFrame* nextKeyframe    = (BakedSkeletalAnimationKeyFrame*)this->getNextKeyFrame();

	if (currentKeyframe != nullptr) {
		GLint boneTransformsHandle = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->GetHandle(SHADER_VARIABLE_VARIABLENAME_boneTransforms);
    	GLCALL(glUniformMatrix4fv, boneTransformsHandle, MAX_BONES, GL_FALSE, glm::value_ptr(currentKeyframe->bakedBoneMatrices[0]));
	}

	if (nextKeyframe == nullptr) {
		nextKeyframe = currentKeyframe;
	}

	if (nextKeyframe != nullptr) {
		GLint otherBoneTransformsHandle = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->GetHandle(SHADER_VARIABLE_VARIABLENAME_otherBoneTransforms);
		GLCALL(glUniformMatrix4fv, otherBoneTransformsHandle, MAX_BONES, GL_FALSE, glm::value_ptr(nextKeyframe->bakedBoneMatrices[0]));
	}

	GLint interpolationHandle = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->GetHandle(SHADER_VARIABLE_VARIABLENAME_interp);
	GLCALL(glUniform2f, interpolationHandle, this->interpolation, 0.0f);
}

void BakedSkeletalAnimationClip::init() {
	this->currentKeyFrameIndex = 0;
}

void BakedSkeletalAnimationClip::destroy() {
}
