//
//  TriggerTransformation.cpp
//  Created by Matt Kaufmann on 02/16/14.
//

#include "ExampleGame/Components/Triggers/TriggerTransformation.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Utilities/MathUtilities.h"

TriggerTransformation::TriggerTransformation() : Triggerable() {
	this->init();
}

TriggerTransformation::TriggerTransformation(Object* object_) : Triggerable(object_) {
	this->init();
}

TriggerTransformation::~TriggerTransformation() {
	this->destroy();
}

void TriggerTransformation::init() {
	this->translation   = ZERO_VEC3;
	this->rotation      = IDENTITY_QUATERNION;
	this->scaling       = glm::vec3(1.0f, 1.0f, 1.0f);
	this->transitTime   = 1.0f;
	this->isTransformed = false;
}

void TriggerTransformation::destroy() {

}

void TriggerTransformation::SetTriggerType(std::string typeStr) {
	Triggerable::SetTriggerType(typeStr);
}

void TriggerTransformation::SetTranslation(float x, float y, float z) {
	glm::vec3 vec(x, y, z);
	this->SetTranslation(vec);
}

void TriggerTransformation::SetTranslation(glm::vec3 vec) {
	this->translation = vec;
}

void TriggerTransformation::SetRotationDegrees(float angleDegrees, float axisX, float axisY, float axisZ) {
	glm::vec3 axis(axisX, axisY, axisZ);
	this->SetRotationRadians(angleDegrees inRadians, axis);
}

void TriggerTransformation::SetRotationDegrees(float angleDegrees, glm::vec3 axis) {
	this->SetRotationRadians(angleDegrees inRadians, axis);
}

void TriggerTransformation::SetRotationRadians(float angleRadians, float axisX, float axisY, float axisZ) {
	glm::vec3 axis(axisX, axisY, axisZ);
	this->SetRotationRadians(angleRadians, axis);
}

void TriggerTransformation::SetRotationRadians(float angleRadians, glm::vec3 axis) {
	this->rotation = glm::angleAxis(angleRadians, axis);
}

void TriggerTransformation::SetScaling(float x, float y, float z) {
	glm::vec3 vec(x, y, z);
	this->SetScaling(vec);
}

void TriggerTransformation::SetScaling(glm::vec3 vec) {
	this->scaling = vec;
}

void TriggerTransformation::HandleMessage(MessageChunk messageChunk) {
	Triggerable::HandleMessage(messageChunk);
}

void TriggerTransformation::SubscribeToMySwitch() {
	Triggerable::SubscribeToMySwitch();
}

void TriggerTransformation::SubscribeToParentSwitch() {
	Triggerable::SubscribeToParentSwitch();
}

void TriggerTransformation::onSwitchToggled(bool /*switchState*/) {
	this->startTransformation(!this->isTransformed);
}

void TriggerTransformation::startTransformation(bool transformed) {
	if (transformed != this->isTransformed) {
		ObjectIdType myId = this->GetObject()->GetId();
		Transform* trans = this->GetObject()->GetComponent<Transform>();

		if (glm::length(this->translation) > ROUNDING_ERROR) {
			glm::vec3 finalPosition = trans->GetPosition();
			if (transformed) {
				finalPosition += this->translation;
			}
			else {
				finalPosition -= this->translation;
			}
			ENGINE->GetTween()->TweenPosition(
				myId,
				finalPosition,
				this->transitTime,
				true,
				TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
				false,
				nullptr
			);
		}

		glm::quat finalOrientation = trans->GetOrientation();
		if (transformed) {
			finalOrientation = finalOrientation * this->rotation;
		}
		else {
			finalOrientation = finalOrientation * glm::inverse(this->rotation);
		}
		ENGINE->GetTween()->TweenOrientation(
			myId,
			finalOrientation,
			this->transitTime,
			true,
			//TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
			false,
			nullptr
		);

		glm::vec3 finalScale = trans->GetScale();
		if (transformed) {
			finalScale = finalScale * this->scaling;
		}
		else {
			finalScale = finalScale / this->scaling;
		}
		ENGINE->GetTween()->TweenScale(
			myId,
			finalScale,
			this->transitTime,
			true,
			//TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
			false,
			nullptr
		);

		this->isTransformed = transformed;
	}
}
