#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/RigidAnimationKeyFrame/RigidAnimationKeyFrame.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/Animation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/RigidAnimationClip/RigidAnimationClip.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Utilities/MathUtilities.h"

RigidAnimationClip::RigidAnimationClip(Animation* parentAnimationComponent_) : AnimationClip(parentAnimationComponent_) {
	this->init();
}

RigidAnimationClip::~RigidAnimationClip() {
	this->destroy();
}

void RigidAnimationClip::InitAnimationClip(std::string urlOfAnimationClip) {
	this->clipDataAsset = ENGINE->GetAssetLibrary()->GetAsset<RigidAnimationClipDataAsset>(urlOfAnimationClip);
	this->SetName(this->clipDataAsset->GetName());
}

void RigidAnimationClip::InitAnimationClip(std::string clipName_, std::vector<AnimationKeyFrame*> animationKeyFramesToAdd) {
	this->clipDataAsset = std::make_shared<RigidAnimationClipDataAsset>();
	for (AnimationKeyFrame* keyFrame : animationKeyFramesToAdd) {
		this->clipDataAsset->AddKeyFrame(keyFrame);
	}
	this->SetName(clipName_);
}

AnimationKeyFrame* RigidAnimationClip::getCurrentKeyFrame() const {
	if (this->currentKeyFrameIndex < this->getNumKeyFrames()) {
		return this->clipDataAsset->GetKeyFrameAtIndex(this->currentKeyFrameIndex);
	}
	return nullptr;
}

AnimationKeyFrame* RigidAnimationClip::getNextKeyFrame() const {
	if ((this->currentKeyFrameIndex + 1) < this->getNumKeyFrames()) {
		return this->clipDataAsset->GetKeyFrameAtIndex(this->currentKeyFrameIndex + 1);
	}
	return nullptr;
}

AnimationKeyFrame* RigidAnimationClip::getCurrentKeyFrameAtInterpolation(float interpolation) {
	RigidAnimationKeyFrame* currentKeyFrame = (RigidAnimationKeyFrame*)this->getCurrentKeyFrame();
	RigidAnimationKeyFrame* nextKeyFrame = (RigidAnimationKeyFrame*)this->getNextKeyFrame();

	if (currentKeyFrame != nullptr && nextKeyFrame != nullptr) {
		float timeBetweenFrames = nextKeyFrame->GetTime() - currentKeyFrame->GetTime();
		float time = currentKeyFrame->GetTime() + timeBetweenFrames * interpolation;
		//
		this->tempKeyFrame->SetTime(time);

		glm::vec3 prevPosition = this->tempKeyFrame->GetTranslation();
		// glm::quat prevRotation = this->tempKeyFrame->GetRotation();

		glm::vec3 translation, scaling;
		glm::quat rotation;

		lerp(translation.x,  currentKeyFrame->GetTranslation().x, nextKeyFrame->GetTranslation().x,  interpolation);
		lerp(translation.y,  currentKeyFrame->GetTranslation().y, nextKeyFrame->GetTranslation().y,  interpolation);
		lerp(translation.z,  currentKeyFrame->GetTranslation().z, nextKeyFrame->GetTranslation().z,  interpolation);

		// glm::quat currentFrameOrientation = currentKeyFrame->GetRotation();
		slerp(rotation, currentKeyFrame->GetRotation(), nextKeyFrame->GetRotation(), interpolation);
		// lerp(rotation.x,  currentKeyFrame->GetRotation().x, nextKeyFrame->GetRotation().x,  interpolation);
		// lerp(rotation.y,  currentKeyFrame->GetRotation().y, nextKeyFrame->GetRotation().y,  interpolation);
		// lerp(rotation.z,  currentKeyFrame->GetRotation().z, nextKeyFrame->GetRotation().z,  interpolation);

		lerp(scaling.x,  currentKeyFrame->GetScaling().x, nextKeyFrame->GetScaling().x,  interpolation);
		lerp(scaling.y,  currentKeyFrame->GetScaling().y, nextKeyFrame->GetScaling().y,  interpolation);
		lerp(scaling.z,  currentKeyFrame->GetScaling().z, nextKeyFrame->GetScaling().z,  interpolation);

		this->tempKeyFrame->SetTranslation(translation);
		this->tempKeyFrame->SetRotation(rotation);
		this->tempKeyFrame->SetScaling(scaling);

		this->tempDeltaKeyFrame->SetTime(time);
		this->tempDeltaKeyFrame->SetTranslation(this->tempKeyFrame->GetTranslation() - prevPosition);
		this->tempDeltaKeyFrame->SetRotation(this->tempKeyFrame->GetRotation());
		this->tempDeltaKeyFrame->SetScaling(this->tempKeyFrame->GetScaling());

		return this->tempDeltaKeyFrame;

	} else {
		// If we couldn't interpolate to next key frame, return current instead (which may or may not be null)
		if (currentKeyFrame != nullptr) {
			glm::vec3 prevPosition;
			if (this->tempKeyFrame != nullptr) {
				prevPosition = this->tempKeyFrame->GetTranslation();
			}

			this->tempKeyFrame->SetTranslation(currentKeyFrame->GetTranslation());
			this->tempKeyFrame->SetRotation(currentKeyFrame->GetRotation());
			this->tempKeyFrame->SetScaling(currentKeyFrame->GetScaling());

			this->tempDeltaKeyFrame->SetTime(currentKeyFrame->GetTime());
			this->tempDeltaKeyFrame->SetTranslation(this->tempKeyFrame->GetTranslation() - prevPosition);
			this->tempDeltaKeyFrame->SetRotation(this->tempKeyFrame->GetRotation());
			this->tempDeltaKeyFrame->SetScaling(this->tempKeyFrame->GetScaling());

			return this->tempDeltaKeyFrame;
		}
	}

	return this->tempKeyFrame;
}

void RigidAnimationClip::reset() {
	this->tempKeyFrame->ResetKeyframeValues();
}

unsigned int RigidAnimationClip::getCurrentKeyFrameIndex() const {
	return this->currentKeyFrameIndex;
}

void RigidAnimationClip::setCurrentKeyFrameIndex(unsigned int index) {
	VERIFY(index < this->getNumKeyFrames(), "Valid key frame index: %d", index);
	this->currentKeyFrameIndex = index;
}

unsigned int RigidAnimationClip::getNumKeyFrames() const {
	return this->clipDataAsset->GetNumKeyFrames();
}

void RigidAnimationClip::init() {
	this->currentKeyFrameIndex = 0;
	this->tempKeyFrame = new RigidAnimationKeyFrame();
	this->tempDeltaKeyFrame = new RigidAnimationKeyFrame();
}

void RigidAnimationClip::destroy() {
}
