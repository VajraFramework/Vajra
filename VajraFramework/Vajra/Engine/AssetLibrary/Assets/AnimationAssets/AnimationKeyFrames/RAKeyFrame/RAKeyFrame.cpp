#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/RAKeyFrame/RAKeyFrame.h"
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

void RigidAnimationKeyFrame::ResetKeyframeValues() {
	this->translation = ZERO_VEC3;
	this->orientationQuat = IDENTITY_QUATERNION;
	this->scaling = ZERO_VEC3;
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
	transform->SetPosition(this->translation);
	transform->SetOrientation(this->orientationQuat);
	transform->SetScale(this->scaling.x, this->scaling.y, this->scaling.z);
}

void RigidAnimationKeyFrame::init() {
	this->ResetKeyframeValues();
}

void RigidAnimationKeyFrame::destroy() {
}
