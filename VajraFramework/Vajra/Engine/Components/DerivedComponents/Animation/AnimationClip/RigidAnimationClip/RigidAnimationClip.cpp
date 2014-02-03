#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/RAKeyFrame/RAKeyFrame.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/Animation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/RigidAnimationClip/RigidAnimationClip.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Utilities/MathUtilities.h"

RigidAnimationClip::RigidAnimationClip(Animation* parentAnimationComponent_) : AnimationClip(parentAnimationComponent_) {
	this->init(parentAnimationComponent_->GetGameObject()->GetTransform());
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

		glm::vec3 translation, scaling;
		glm::quat rotation;

		lerp(translation.x,  currentKeyFrame->GetTranslation().x, nextKeyFrame->GetTranslation().x,  interpolation);
		lerp(translation.y,  currentKeyFrame->GetTranslation().y, nextKeyFrame->GetTranslation().y,  interpolation);
		lerp(translation.z,  currentKeyFrame->GetTranslation().z, nextKeyFrame->GetTranslation().z,  interpolation);

		slerp(rotation, currentKeyFrame->GetRotation(), nextKeyFrame->GetRotation(), interpolation);
		// lerp(rotation.x,  currentKeyFrame->GetRotation().x, nextKeyFrame->GetRotation().x,  interpolation);
		// lerp(rotation.y,  currentKeyFrame->GetRotation().y, nextKeyFrame->GetRotation().y,  interpolation);
		// lerp(rotation.z,  currentKeyFrame->GetRotation().z, nextKeyFrame->GetRotation().z,  interpolation);

		lerp(scaling.x,  currentKeyFrame->GetScaling().x, nextKeyFrame->GetScaling().x,  interpolation);
		lerp(scaling.y,  currentKeyFrame->GetScaling().y, nextKeyFrame->GetScaling().y,  interpolation);
		lerp(scaling.z,  currentKeyFrame->GetScaling().z, nextKeyFrame->GetScaling().z,  interpolation);

		this->tempKeyFrame->SetTranslation(translation + this->initialPosition);
		this->tempKeyFrame->SetRotation(rotation * this->initialOrientation);
		this->tempKeyFrame->SetScaling(scaling);

		return this->tempKeyFrame;

	} else {
		// If we couldn't interpolate to next key frame, return current instead (which may or may not be null)
		if (currentKeyFrame != nullptr) {

			this->tempKeyFrame->SetTranslation(currentKeyFrame->GetTranslation() + this->initialPosition);
			this->tempKeyFrame->SetRotation(currentKeyFrame->GetRotation() * this->initialOrientation);
			this->tempKeyFrame->SetScaling(currentKeyFrame->GetScaling());

			return this->tempKeyFrame;
		}
	}

	ASSERT(0, "Unreachable code");
	return nullptr;
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

void RigidAnimationClip::init(Transform* initialTransform) {
	this->currentKeyFrameIndex = 0;
	this->tempKeyFrame = new RigidAnimationKeyFrame();

	// Save the original position, orientation, and scale of the game object so that we can apply the animation as an offset over it
	// TODO [Hack] CHange these to Get*World() functions instead:
	this->initialPosition    = initialTransform->GetPositionWorld();
	this->initialOrientation = initialTransform->GetOrientation();
	this->initialScale       = initialTransform->GetScale();
}

void RigidAnimationClip::destroy() {
}
