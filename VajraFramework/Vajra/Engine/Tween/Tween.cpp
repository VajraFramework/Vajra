#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/RAKeyFrame/RAKeyFrame.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/RigidAnimationClip/RigidAnimationClip.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/RigidAnimation/RigidAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/MathUtilities.h"

#include <cstdio>

Tween::Tween() {
	this->init();
}

Tween::~Tween() {
	this->destroy();
}

void Tween::createNewTransformTween(ObjectIdType gameObjectId, TransformTweenTarget transformTweenTarget,
		glm::vec3 from_v, glm::vec3 to_v, float time,
		void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName),
		bool looping,
		TweenTranslationCurveType curveType) {
	OnGoingTransformTweenDetails* newTransformTweenDetails = new OnGoingTransformTweenDetails();
	newTransformTweenDetails->tweenTarget  = transformTweenTarget;
	newTransformTweenDetails->from_v       = from_v;
	newTransformTweenDetails->to_v         = to_v;
	newTransformTweenDetails->current_v    = from_v;
	newTransformTweenDetails->callback     = callback;
	newTransformTweenDetails->looping      = looping;
	newTransformTweenDetails->gameObjectId = gameObjectId;
	newTransformTweenDetails->curveType    = curveType;
	newTransformTweenDetails->totalTime    = time;
	ASSERT(time != 0, "TimePeriod to tween over is not zero");

	switch (transformTweenTarget) {
	case TRANSFORM_TWEEN_TARGET_POSITION: { this->ongoingPositionTweens[gameObjectId] = newTransformTweenDetails; } break;
	case TRANSFORM_TWEEN_TARGET_SCALE:    { this->ongoingScaleTweens[gameObjectId]    = newTransformTweenDetails; } break;
	default:                              { ASSERT(0, "Unknown TransformTweenTarget %d", transformTweenTarget);   } break;
	}
}

void Tween::createNewTransformTween(ObjectIdType gameObjectId, TransformTweenTarget transformTweenTarget,
		glm::quat from_q, glm::quat to_q, float time,
		void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName),
		bool looping) {
	OnGoingTransformTweenDetails* newTransformTweenDetails = new OnGoingTransformTweenDetails();
	newTransformTweenDetails->tweenTarget  = transformTweenTarget;
	newTransformTweenDetails->from_q       = from_q;
	newTransformTweenDetails->to_q         = to_q;
	newTransformTweenDetails->current_q    = from_q;
	newTransformTweenDetails->callback     = callback;
	newTransformTweenDetails->looping      = looping;
	newTransformTweenDetails->gameObjectId = gameObjectId;
	newTransformTweenDetails->totalTime    = time;
	ASSERT(time != 0, "TimePeriod to tween over is not zero");

	switch (transformTweenTarget) {
	case TRANSFORM_TWEEN_TARGET_ORIENTATION: { this->ongoingOrientationTweens[gameObjectId] = newTransformTweenDetails; } break;
	default:                                 { ASSERT(0, "Unknown TransformTweenTarget %d", transformTweenTarget);      } break;
	}
}

void Tween::TweenPosition(ObjectIdType gameObjectId, glm::vec3 finalPosition, float time, bool cancelCurrentTween /* = true */, TweenTranslationCurveType curveType /* = TWEEN_TRANSLATION_CURVE_TYPE_LINEAR */, bool looping /* = false */, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		this->TweenPosition(gameObjectId, gameObject->GetTransform()->GetPosition(), finalPosition, time, cancelCurrentTween, curveType, looping, callback);
	}
}

void Tween::TweenPosition(ObjectIdType gameObjectId, glm::vec3 initialPosition, glm::vec3 finalPosition, float time, bool cancelCurrentTween /* = true */, TweenTranslationCurveType curveType /* = TWEEN_TRANSLATION_CURVE_TYPE_LINEAR */, bool looping /* = false */, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		if (this->IsTweening_transform(gameObjectId, TRANSFORM_TWEEN_TARGET_POSITION)) {
			if (!cancelCurrentTween) {
				FRAMEWORK->GetLogger()->dbglog("\nWARNING GameObject %d is already tweening on position, ignoring tween command", gameObject->GetId());
				return;
			} else {
				// Cancel current tween
				this->cancelOngoingTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_POSITION);
			}
		}

		this->createNewTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_POSITION, initialPosition, finalPosition, time, callback, looping, curveType);

	} else {
		FRAMEWORK->GetLogger()->dbglog("\nTrying to tween null GameObject");
	}
}

void Tween::TweenOrientation(ObjectIdType gameObjectId, glm::quat finalOrientation, float time, bool cancelCurrentTween /* = true */, bool looping /* = false */, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) /* = 0 */) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		this->TweenOrientation(gameObjectId, gameObject->GetTransform()->GetOrientation(), finalOrientation, time, cancelCurrentTween, looping, callback);
	}
}

void Tween::TweenOrientation(ObjectIdType gameObjectId, glm::quat initialOrientation, glm::quat finalOrientation, float time, bool cancelCurrentTween /* = true */, bool looping /* = false */,
		void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) /* = 0 */) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		if (this->IsTweening_transform(gameObjectId, TRANSFORM_TWEEN_TARGET_ORIENTATION)) {
			if (!cancelCurrentTween) {
				FRAMEWORK->GetLogger()->dbglog("\nWARNING GameObject %d is already tweening on orientation, ignoring tween command", gameObject->GetId());
				return;
			} else {
				// Cancel current tween
				this->cancelOngoingTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_ORIENTATION);
			}
		}

		this->createNewTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_ORIENTATION, initialOrientation, finalOrientation, time, callback, looping);

	} else {
		FRAMEWORK->GetLogger()->dbglog("\nTrying to tween null GameObject");
	}
}

void Tween::TweenScale(ObjectIdType gameObjectId, glm::vec3 finalScale, float time, bool cancelCurrentTween /* = true */, bool looping /* = false */, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		this->TweenScale(gameObjectId, gameObject->GetTransform()->GetScale(), finalScale, time, cancelCurrentTween, looping, callback);
	}
}

void Tween::TweenScale(ObjectIdType gameObjectId, glm::vec3 initialScale, glm::vec3 finalScale, float time, bool cancelCurrentTween /* = true */, bool looping /* = false */,
		void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		if (this->IsTweening_transform(gameObjectId, TRANSFORM_TWEEN_TARGET_SCALE)) {
			if (!cancelCurrentTween) {
				FRAMEWORK->GetLogger()->dbglog("\nWARNING GameObject %d is already tweening on scale, ignoring tween command", gameObject->GetId());
				return;
			} else {
				// Cancel current tween
				this->cancelOngoingTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_SCALE);
			}
		}

		this->createNewTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_SCALE, initialScale, finalScale, time, callback, looping);

	} else {
		FRAMEWORK->GetLogger()->dbglog("\nTrying to tween null GameObject");
	}
}

void Tween::TweenToNumber(float fromNumber, float toNumber, float timePeriod, bool cancelCurrentTween, bool looping, bool continuousUpdates, std::string tweenName,
						  void (*callback)(float fromNumber, float toNumber, float currentNumber, std::string tweenName)) {
	bool resettingExistingTween = false;
	OnGoingNumberTweenDetails* newNumberTween = nullptr;
	if (this->IsTweening_number(tweenName)) {
		if (cancelCurrentTween) {
			this->cancelOngoingNumberTween(tweenName);
			newNumberTween = new OnGoingNumberTweenDetails();
		} else {
			// Reset the tween:
			newNumberTween = this->ongoingNumberTweens[tweenName];
			newNumberTween->ResetTween();
			resettingExistingTween = true;
		}
	} else {
		newNumberTween = new OnGoingNumberTweenDetails();
	}
	ASSERT(newNumberTween != nullptr, "Number tween not nullptr");
	newNumberTween->fromNumber                = fromNumber;
	newNumberTween->currentNumber             = fromNumber;
	newNumberTween->toNumber                  = toNumber;
	newNumberTween->totalTime                 = timePeriod;
	newNumberTween->callback                  = callback;
	newNumberTween->continuousUpdates         = continuousUpdates;
	newNumberTween->tweenName                 = tweenName;
	newNumberTween->looping                   = looping;
	ASSERT(timePeriod != 0, "TimePeriod to tween over is not zero");

	if (!resettingExistingTween) {
		this->ongoingNumberTweens[tweenName] = newNumberTween;
	}
}

void Tween::updateTweens() {
	float deltaTime = ENGINE->GetTimer()->GetDeltaFrameTime();
	this->updateNumberTweens(deltaTime);
	this->updateTransformTweens(deltaTime);
}

void Tween::updateTransformTweens(float deltaTime) {
	this->updateTransformTweens_internal(deltaTime, &this->ongoingPositionTweens);
	this->updateTransformTweens_internal(deltaTime, &this->ongoingOrientationTweens);
	this->updateTransformTweens_internal(deltaTime, &this->ongoingScaleTweens);
}

void Tween::updateTransformTweens_internal(float deltaTime, std::map<ObjectIdType, OnGoingTransformTweenDetails*>* ongoingTweens) {
	std::vector<ObjectIdType> gameObjectIdsOfCompletedTweens;
	for (auto it = ongoingTweens->begin(); it != ongoingTweens->end(); ++it) {
		OnGoingTransformTweenDetails* tweenDetails = it->second;

		if (!tweenDetails->StepTween(deltaTime)) {
			gameObjectIdsOfCompletedTweens.push_back(tweenDetails->gameObjectId);
		}
	}
	// Erase completed tweens:
	for (ObjectIdType gameObjectId : gameObjectIdsOfCompletedTweens) {
		ongoingTweens->erase(gameObjectId);
	}
}

void Tween::updateNumberTweens(float deltaTime) {
	std::vector<std::string> completedTweensToBeRemoved;
	for (auto it = this->ongoingNumberTweens.begin(); it != this->ongoingNumberTweens.end(); ++it) {
		OnGoingNumberTweenDetails* tweenDetails = it->second;

		if (!tweenDetails->StepTween(deltaTime)) {
			completedTweensToBeRemoved.push_back(tweenDetails->tweenName);
		}
	}
	// Erase completed tweens:
	for (std::string tweenName : completedTweensToBeRemoved) {
		this->ongoingNumberTweens.erase(tweenName);
	}
}

bool Tween::IsTweening_transform(ObjectIdType gameObjectId, TransformTweenTarget transformTweenTarget) {
	switch (transformTweenTarget) {
	case TRANSFORM_TWEEN_TARGET_POSITION:
		return (this->ongoingPositionTweens.find(gameObjectId) != this->ongoingPositionTweens.end());
	case TRANSFORM_TWEEN_TARGET_ORIENTATION:
		return (this->ongoingOrientationTweens.find(gameObjectId) != this->ongoingOrientationTweens.end());
	case TRANSFORM_TWEEN_TARGET_SCALE:
		return (this->ongoingScaleTweens.find(gameObjectId) != this->ongoingScaleTweens.end());
	default:
		ASSERT(0, "Unknown TransformTweenTarget %d", transformTweenTarget);
	}
	return false;
}

bool Tween::IsTweening_number(std::string tweenName) {
	return (this->ongoingNumberTweens.find(tweenName) != this->ongoingNumberTweens.end());
}

void Tween::cancelOngoingTransformTween(ObjectIdType gameObjectId, TransformTweenTarget tweenTarget) {

	switch (tweenTarget) {
	case TRANSFORM_TWEEN_TARGET_POSITION: {
		this->ongoingPositionTweens.erase(gameObjectId);
	} break;

	case TRANSFORM_TWEEN_TARGET_ORIENTATION: {
		this->ongoingOrientationTweens.erase(gameObjectId);
	} break;

	case TRANSFORM_TWEEN_TARGET_SCALE: {
		this->ongoingScaleTweens.erase(gameObjectId);
	} break;

	default: ASSERT(0, "Unknown TransformTweenTarget %d", tweenTarget);
	}
}

void Tween::cancelOngoingNumberTween(std::string tweenName) {
	this->ongoingNumberTweens.erase(tweenName);
}

void Tween::CancelPostitionTween(ObjectIdType gameObjectId) {
	this->cancelOngoingTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_POSITION);
}

void Tween::CancelOrientationTween(ObjectIdType gameObjectId) {
	this->cancelOngoingTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_ORIENTATION);
}

void Tween::CancelScaleTween(ObjectIdType gameObjectId) {
	this->cancelOngoingTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_SCALE);
}

void Tween::CancelNumberTween(std::string tweenName) {
	this->cancelOngoingNumberTween(tweenName);
}

void Tween::init() {
}

void Tween::destroy() {
}

////////////////////////////////////////////////////////////////////////////////

void OnGoingTransformTweenDetails::ResetTween() {
	this->current_v = this->from_v;
	this->current_q = this->from_q;
	this->currentTime = 0.0f;
}

void OnGoingNumberTweenDetails::ResetTween() {
	this->currentNumber = this->fromNumber;
}


#define TWEEN_PARABOLA_a 0.08f                  // From x = 4ay^2

bool OnGoingTransformTweenDetails::StepTween(float deltaTime) {
	float curTime = this->currentTime + deltaTime;
	float interp = curTime / this->totalTime;
	clamp(interp, 0.0f, 1.0f);
	this->currentTime = curTime;

	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->gameObjectId);
	ASSERT(gameObject != nullptr, "Found game object (%d) on which to apply tween", this->gameObjectId);
	Transform* transform = gameObject->GetTransform();

	switch (this->tweenTarget) {
	case TRANSFORM_TWEEN_TARGET_POSITION: {
		if (this->curveType == TWEEN_TRANSLATION_CURVE_TYPE_LINEAR) {
			lerp(this->current_v, this->from_v, this->to_v, interp);
		} else if (this->curveType == TWEEN_TRANSLATION_CURVE_TYPE_PARABOLA) {
			parabolaerp(this->current_v, this->from_v, this->to_v, TWEEN_PARABOLA_a, interp);
		}
		transform->SetPosition(this->current_v);
	} break;

	case TRANSFORM_TWEEN_TARGET_ORIENTATION: {
		slerp(this->current_q, this->from_q, this->to_q, interp);
		transform->SetOrientation(this->current_q);
	} break;

	case TRANSFORM_TWEEN_TARGET_SCALE: {
		lerp(this->current_v, this->from_v, this->to_v, interp);
		transform->SetScale(this->current_v);
	} break;

	default: { ASSERT(0, "Unknown TransformTweenTarget %d", this->tweenTarget);   } break;
	}

	if (curTime >= this->totalTime) {
		if (this->callback != 0) {
			this->callback(gameObject->GetId(), this->userDefinedTweenName);
		}
		if (this->looping) {
			this->ResetTween();
		} else {
			// Tween got over
			return false;
		}
	}
	// Tween still in progress
	return true;
}

bool OnGoingNumberTweenDetails::StepTween(float deltaTime) {
	float newNumber = this->currentNumber + deltaTime * (this->toNumber - this->fromNumber) / this->totalTime;
	this->currentNumber = newNumber;
	if (newNumber > this->toNumber || this->continuousUpdates) {
		ASSERT(this->callback != 0, "Callback not 0");
		clamp(newNumber, this->fromNumber, this->toNumber);
		this->callback(this->fromNumber, this->toNumber, newNumber, this->tweenName);
	}

	if (newNumber > this->toNumber) {
		if (this->looping) {
			this->ResetTween();
		} else {
			// Tween got over
			return false;
		}
	}
	// Tween still in progress
	return true;
}

////////////////////////////////////////////////////////////////////////////////
