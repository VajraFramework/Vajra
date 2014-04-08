//
//  TriggerTransformation.cpp
//  Created by Matt Kaufmann on 02/16/14.
//

#include "ExampleGame/Components/Triggers/TriggerTransformation.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
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
}

void TriggerTransformation::destroy() {

}

void TriggerTransformation::SetTriggerType(std::string typeStr) {
	Triggerable::SetTriggerType(typeStr);
}

void TriggerTransformation::SetActivationAudio(std::string audioStr) {
	Triggerable::SetActivationAudio(audioStr);
}

void TriggerTransformation::SetDeactivationAudio(std::string audioStr) {
	Triggerable::SetDeactivationAudio(audioStr);
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

void TriggerTransformation::onSwitchToggled(bool switchState) {
	this->startTransformation(switchState);
}

void TriggerTransformation::startTransformation(bool transformed) {
	if (transformed != this->isToggled) {
		this->startTranslation(transformed);
		this->startRotation(transformed);
		this->startScaling(transformed);
	}
}

void TriggerTransformation::startTranslation(bool transformed) {
	ObjectIdType myId = this->GetObject()->GetId();
	Transform* trans = this->GetObject()->GetComponent<Transform>();

	// Check if there's already a tween going on.
	OnGoingTransformTweenDetails* translationDetails = ENGINE->GetTween()->GetOnGoingTransformTweenDetails(myId, TRANSFORM_TWEEN_TARGET_POSITION);
	if (translationDetails == nullptr) {
		// Don't do a tween if there's no translation
		if (glm::length(this->translation) > ROUNDING_ERROR) {
			glm::vec3 finalPosition = trans->GetPosition();
			if (transformed) {
				finalPosition += this->translation;
			}
			else {
				finalPosition -= this->translation;
			}

			float tweenTime = this->transitTime;

			ENGINE->GetTween()->TweenPosition(
				myId,
				finalPosition,
				tweenTime,
				true,
				INTERPOLATION_TYPE_LINEAR,
				false,
				nullptr
			);
		}
	}
	else {
		// Reverse the current tween
		float tweenTime = this->transitTime - translationDetails->totalTime + translationDetails->currentTime;
		glm::vec3 diff;
		if (transformed) {
			diff = this->translation * (tweenTime / this->transitTime);
		}
		else {
			diff = this->translation * (-tweenTime / this->transitTime);
		}

		glm::vec3 finalPosition = trans->GetPosition() + diff;

		if (tweenTime > 0.0f) {
			ENGINE->GetTween()->TweenPosition(
				myId,
				finalPosition,
				tweenTime,
				true,
				INTERPOLATION_TYPE_LINEAR,
				false,
				nullptr
			);
		}
		else {
			// Well that's strange. Just cancel the tween.
			ENGINE->GetTween()->CancelPostitionTween(myId);
		}
	}
}

void TriggerTransformation::startRotation(bool transformed) {
	ObjectIdType myId = this->GetObject()->GetId();
	Transform* trans = this->GetObject()->GetComponent<Transform>();

	// Check if there's already a tween going on.
	OnGoingTransformTweenDetails* rotationDetails = ENGINE->GetTween()->GetOnGoingTransformTweenDetails(myId, TRANSFORM_TWEEN_TARGET_ORIENTATION);
	if (rotationDetails == nullptr) {
		// Don't do a tween if there's no rotation
		if (true) {
			glm::quat finalOrientation = trans->GetOrientation();
			if (transformed) {
				finalOrientation = finalOrientation * this->rotation;
			}
			else {
				finalOrientation = finalOrientation * glm::inverse(this->rotation);
			}

			float tweenTime = this->transitTime;

			ENGINE->GetTween()->TweenOrientation(
				myId,
				finalOrientation,
				tweenTime,
				true,
				false,
				nullptr
			);
		}
	}
	else {
		// Reverse the current tween
		float tweenTime = this->transitTime - rotationDetails->totalTime + rotationDetails->currentTime;
		float angle = glm::angle(this->rotation);
		glm::vec3 axis = glm::axis(this->rotation);
		glm::quat diff;
		if (transformed) {
			diff = glm::angleAxis(angle * (tweenTime / this->transitTime), axis);
		}
		else {
			diff = glm::angleAxis(angle * (tweenTime / this->transitTime), -axis);
		}
		glm::quat finalOrientation = trans->GetOrientation() * diff;

		if (tweenTime > 0.0f) {
			ENGINE->GetTween()->TweenOrientation(
				myId,
				finalOrientation,
				tweenTime,
				true,
				false,
				nullptr
			);
		}
		else {
			// Well that's strange. Just cancel the tween.
			ENGINE->GetTween()->CancelPostitionTween(myId);
		}
	}
}

void TriggerTransformation::startScaling(bool transformed) {
	ObjectIdType myId = this->GetObject()->GetId();
	Transform* trans = this->GetObject()->GetComponent<Transform>();

	// Check if there's already a tween going on.
	OnGoingTransformTweenDetails* scalingDetails = ENGINE->GetTween()->GetOnGoingTransformTweenDetails(myId, TRANSFORM_TWEEN_TARGET_SCALE);
	if (scalingDetails == nullptr) {
		// Don't do a tween if there's no scaling
		if (true) {
			glm::vec3 finalScale = trans->GetScale();
			if (transformed) {
				finalScale = finalScale * this->scaling;
			}
			else {
				finalScale = finalScale / this->scaling;
			}

			float tweenTime = this->transitTime;

			ENGINE->GetTween()->TweenScale(
				myId,
				finalScale,
				tweenTime,
				true,
				false,
				nullptr
			);
		}
	}
	else {
		// Reverse the current tween
		float tweenTime = this->transitTime - scalingDetails->totalTime + scalingDetails->currentTime;
		glm::vec3 diff  = (scalingDetails->current_v - scalingDetails->to_v) * (tweenTime / (this->transitTime - tweenTime));
		glm::vec3 finalScale = trans->GetScale() + diff;

		if (tweenTime > 0.0f) {
			ENGINE->GetTween()->TweenScale(
				myId,
				finalScale,
				tweenTime,
				true,
				false,
				nullptr
			);
		}
		else {
			// Well that's strange. Just cancel the tween.
			ENGINE->GetTween()->CancelPostitionTween(myId);
		}
	}
}
