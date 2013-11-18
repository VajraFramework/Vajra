#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/RigidAnimationKeyFrame/RigidAnimationKeyFrame.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/MathUtilities.h"

RigidAnimationKeyFrame::RigidAnimationKeyFrame() {
	this->init();
}

RigidAnimationKeyFrame::~RigidAnimationKeyFrame() {
	this->destroy();
}

void RigidAnimationKeyFrame::SetTranslation(glm::vec3 v) {
	this->translation = v;
}

void RigidAnimationKeyFrame::SetRotation(glm::quat q) {
	this->orientationQuat = q;
}

void RigidAnimationKeyFrame::SetScaling(glm::vec3 v) {
	this->scaling = v;
}

void RigidAnimationKeyFrame::ApplyKeyFrameToTransform(Transform* transform) const {
	transform->Translate(this->translation);
	// TODO [Implement] Use Rotate() here so that it is relative to the gameObject's orientation
	transform->SetOrientation(this->orientationQuat);
	// TODO [Implement] Use Scale() here so that it is relative to the gameObject's scale
	transform->SetScale(this->scaling.x, this->scaling.y, this->scaling.z);
}

void RigidAnimationKeyFrame::init() {
}

void RigidAnimationKeyFrame::destroy() {
}
