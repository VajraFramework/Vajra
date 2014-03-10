#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1I1F.h"
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
}

Tween::~Tween() {
	this->destroy();
}

void Tween::createNewTransformTween(ObjectIdType gameObjectId, TransformTweenTarget transformTweenTarget,
		glm::vec3 from_v, glm::vec3 to_v, float time,
		void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName),
		bool looping,
		InterpolationType_t interpolationType) {
	OnGoingTransformTweenDetails* newTransformTweenDetails = new OnGoingTransformTweenDetails();
	newTransformTweenDetails->tweenTarget  = transformTweenTarget;
	newTransformTweenDetails->from_v       = from_v;
	newTransformTweenDetails->to_v         = to_v;
	newTransformTweenDetails->current_v    = from_v;
	newTransformTweenDetails->callback     = callback;
	newTransformTweenDetails->looping      = looping;
	newTransformTweenDetails->gameObjectId = gameObjectId;
	newTransformTweenDetails->interpolationType    = interpolationType;
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

void Tween::TweenPosition(ObjectIdType gameObjectId, glm::vec3 finalPosition, float time, bool cancelCurrentTween /* = true */, InterpolationType_t interpolationType /* = INTERPOLATION_TYPE_LINEAR */, bool looping /* = false */, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		this->TweenPosition(gameObjectId, gameObject->GetTransform()->GetPosition(), finalPosition, time, cancelCurrentTween, interpolationType, looping, callback);
	}
}

void Tween::TweenPosition(ObjectIdType gameObjectId, glm::vec3 initialPosition, glm::vec3 finalPosition, float time, bool cancelCurrentTween /* = true */, InterpolationType_t interpolationType /* = INTERPOLATION_TYPE_LINEAR */, bool looping /* = false */, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
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

		this->createNewTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_POSITION, initialPosition, finalPosition, time, callback, looping, interpolationType);

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

void Tween::TweenToNumber(float fromNumber, float toNumber, float timePeriod, InterpolationType_t interpolationType, bool cancelCurrentTween, bool looping, bool continuousUpdates, std::string tweenName,
						  NumberTweenAffliationSceneGraph affiliation,
						  MessageData1S1I1F* userParams,
						  void (*callback)(float fromNumber, float toNumber, float currentNumber, std::string tweenName, MessageData1S1I1F* userParams)) {
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
	newNumberTween->toNumber                  = toNumber;
	newNumberTween->currentTime               = 0.0f;
	newNumberTween->totalTime                 = timePeriod;
	newNumberTween->interpolationType         = interpolationType;
	newNumberTween->affiliation               = affiliation;
	newNumberTween->callback                  = callback;
	newNumberTween->userParams                = userParams;
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
	OnGoingTransformTweenDetails* tweenDetails = this->GetOnGoingTransformTweenDetails(gameObjectId, tweenTarget);
	if (tweenDetails != nullptr) {
		tweenDetails->canceled = true;
	}
}

void Tween::pauseOngoingTransformTween(ObjectIdType gameObjectId, TransformTweenTarget tweenTarget) {
	OnGoingTransformTweenDetails* tweenDetails = this->GetOnGoingTransformTweenDetails(gameObjectId, tweenTarget);
	if (tweenDetails != nullptr) {
		tweenDetails->Pause();
	}
}

void Tween::resumeOngoingTransformTween(ObjectIdType gameObjectId, TransformTweenTarget tweenTarget) {
	OnGoingTransformTweenDetails* tweenDetails = this->GetOnGoingTransformTweenDetails(gameObjectId, tweenTarget);
	if (tweenDetails != nullptr) {
		tweenDetails->Resume();
	}
}


void Tween::cancelOngoingNumberTween(std::string tweenName) {
	OnGoingNumberTweenDetails* tweenDetails = this->GetOnGoingNumberTweenDetails(tweenName);
	if (tweenDetails != nullptr) {
		tweenDetails->canceled = true;
	}
}

void Tween::pauseOngoingNumberTween(std::string tweenName) {
	OnGoingNumberTweenDetails* tweenDetails = this->GetOnGoingNumberTweenDetails(tweenName);
	if (tweenDetails != nullptr) {
		tweenDetails->Pause();
	}
}

void Tween::resumeOngoingNumberTween(std::string tweenName) {
	OnGoingNumberTweenDetails* tweenDetails = this->GetOnGoingNumberTweenDetails(tweenName);
	if (tweenDetails != nullptr) {
		tweenDetails->Resume();
	}
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



void Tween::PausePostitionTween(ObjectIdType gameObjectId) {
	this->pauseOngoingTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_POSITION);
}

void Tween::PauseOrientationTween(ObjectIdType gameObjectId) {
	this->pauseOngoingTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_ORIENTATION);
}

void Tween::PauseScaleTween(ObjectIdType gameObjectId) {
	this->pauseOngoingTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_SCALE);
}

void Tween::PauseNumberTween(std::string tweenName) {
	this->pauseOngoingNumberTween(tweenName);
}

void Tween::PauseNumberTweensByAffiliation(NumberTweenAffliationSceneGraph affiliation) {
	for (auto it = this->ongoingNumberTweens.begin(); it != this->ongoingNumberTweens.end(); ++it) {
		OnGoingNumberTweenDetails* tweenDetails = it->second;
		if (tweenDetails->affiliation == affiliation) {
			tweenDetails->Pause();
		}
	}
}



void Tween::ResumePostitionTween(ObjectIdType gameObjectId) {
	this->resumeOngoingTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_POSITION);
}

void Tween::ResumeOrientationTween(ObjectIdType gameObjectId) {
	this->resumeOngoingTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_ORIENTATION);
}

void Tween::ResumeScaleTween(ObjectIdType gameObjectId) {
	this->resumeOngoingTransformTween(gameObjectId, TRANSFORM_TWEEN_TARGET_SCALE);
}

void Tween::ResumeNumberTween(std::string tweenName) {
	this->resumeOngoingNumberTween(tweenName);
}

void Tween::ResumeNumberTweensByAffiliation(NumberTweenAffliationSceneGraph affiliation) {
	for (auto it = this->ongoingNumberTweens.begin(); it != this->ongoingNumberTweens.end(); ++it) {
		OnGoingNumberTweenDetails* tweenDetails = it->second;
		if (tweenDetails->affiliation == affiliation) {
			tweenDetails->Resume();
		}
	}
}


OnGoingTransformTweenDetails* Tween::GetOnGoingTransformTweenDetails(ObjectIdType gameObjectId, TransformTweenTarget tweenTarget) {
		OnGoingTransformTweenDetails* returnResult = nullptr;

		switch (tweenTarget) {
		case TRANSFORM_TWEEN_TARGET_POSITION: {
			if (this->ongoingPositionTweens.find(gameObjectId) != this->ongoingPositionTweens.end()) {
				returnResult = this->ongoingPositionTweens[gameObjectId];
			}
		} break;

		case TRANSFORM_TWEEN_TARGET_ORIENTATION: {
			if (this->ongoingOrientationTweens.find(gameObjectId) != this->ongoingOrientationTweens.end()) {
				returnResult = this->ongoingOrientationTweens[gameObjectId];
			}
		} break;

		case TRANSFORM_TWEEN_TARGET_SCALE: {
			if (this->ongoingScaleTweens.find(gameObjectId) != this->ongoingScaleTweens.end()) {
				returnResult = this->ongoingScaleTweens[gameObjectId];
			}
		} break;

		default: { ASSERT(0, "Unknown TransformTweenTarget %d", tweenTarget);   } break;
		}

		return returnResult;
}

OnGoingNumberTweenDetails* Tween::GetOnGoingNumberTweenDetails(std::string tweenName) {
		OnGoingNumberTweenDetails* returnResult = nullptr;
		if (this->ongoingNumberTweens.find(tweenName) != this->ongoingNumberTweens.end()) {
			returnResult = this->ongoingNumberTweens[tweenName];
		}
		return returnResult;
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
	this->currentTime = 0.0f;
}


#define TWEEN_PARABOLA_a 0.08f                  // From x = 4ay^2

bool OnGoingTransformTweenDetails::StepTween(float deltaTime) {
	if (this->isPaused) {
		// Tween still in progress, but paused
		return true;
	}

	if (this->canceled) {
		// Tween was canceled -- have it remove itself:
		return false;
	}

	float curTime = this->currentTime + deltaTime;
	float interp = curTime / this->totalTime;
	clamp(interp, 0.0f, 1.0f);
	this->currentTime = curTime;

	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->gameObjectId);
	if (gameObject != nullptr) {
		Transform* transform = gameObject->GetTransform();

		switch (this->tweenTarget) {
		case TRANSFORM_TWEEN_TARGET_POSITION: {

			switch (this->interpolationType) {
			case INTERPOLATION_TYPE_LINEAR:
				lerp(this->current_v, this->from_v, this->to_v, interp); break;
			case INTERPOLATION_TYPE_CUBIC:
				cubicerp(this->current_v, this->from_v, this->to_v, this->currentTime, totalTime); break;
			case INTERPOLATION_TYPE_CUBIC_INVERSE:
				cubicinverseerp(this->current_v, this->from_v, this->to_v, this->currentTime, totalTime); break;
			case INTERPOLATION_TYPE_SINE:
				sineerp(this->current_v, this->from_v, this->to_v, this->currentTime, totalTime); break;
			case INTERPOLATION_TYPE_EXPONENTIAL:
				exponentialerp(this->current_v, this->from_v, this->to_v, this->currentTime, totalTime); break;
			case INTERPOLATION_TYPE_OVERSHOOT:
				overshooterp(this->current_v, this->from_v, this->to_v, this->currentTime, totalTime); break;
			case INTERPOLATION_TYPE_PARABOLA:
				parabolaerp(this->current_v, this->from_v, this->to_v, TWEEN_PARABOLA_a, interp); break;
			default:
				ASSERT(0, "Valid interpolation type, %d", this->interpolationType); break;
			}
			transform->SetPosition(this->current_v);
		} break;

		case TRANSFORM_TWEEN_TARGET_ORIENTATION: {
			slerp(this->current_q, this->from_q, this->to_q, interp);
			transform->SetOrientation(this->current_q);
		} break;

		case TRANSFORM_TWEEN_TARGET_SCALE: {
			switch (this->interpolationType) {
			case INTERPOLATION_TYPE_LINEAR:
				lerp(this->current_v, this->from_v, this->to_v, interp); break;
			case INTERPOLATION_TYPE_CUBIC:
				cubicerp(this->current_v, this->from_v, this->to_v, this->currentTime, totalTime); break;
			case INTERPOLATION_TYPE_CUBIC_INVERSE:
				cubicinverseerp(this->current_v, this->from_v, this->to_v, this->currentTime, totalTime); break;
			case INTERPOLATION_TYPE_SINE:
				sineerp(this->current_v, this->from_v, this->to_v, this->currentTime, totalTime); break;
			case INTERPOLATION_TYPE_EXPONENTIAL:
				exponentialerp(this->current_v, this->from_v, this->to_v, this->currentTime, totalTime); break;
			case INTERPOLATION_TYPE_OVERSHOOT:
				overshooterp(this->current_v, this->from_v, this->to_v, this->currentTime, totalTime); break;
			case INTERPOLATION_TYPE_PARABOLA:
				ASSERT(0, "Valid interpolation type, %d (no _PARABOLA for scale tween)", this->interpolationType); break;
			default:
				ASSERT(0, "Valid interpolation type, %d", this->interpolationType); break;
			}
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

	} else {
		
		// Did not find game object on which to apply tween, probably because it was deleted. Have the tween canceled.
		return false;
	}

	return false;
}

bool OnGoingNumberTweenDetails::StepTween(float deltaTime) {
	if (this->isPaused) {
		// Tween still in progress, but paused
		return true;
	}

	if (this->canceled) {
		// Tween was canceled -- have it remove itself:
		return false;
	}

	// float newNumber = this->currentNumber + deltaTime * (this->toNumber - this->fromNumber) / this->totalTime;
	float newNumber;

	switch (this->interpolationType) {
	case INTERPOLATION_TYPE_LINEAR:
		lerp(newNumber, this->fromNumber, this->toNumber, this->currentTime); break;
	case INTERPOLATION_TYPE_CUBIC:
		cubicerp(newNumber, this->fromNumber, this->toNumber, this->currentTime, this->totalTime); break;
	case INTERPOLATION_TYPE_CUBIC_INVERSE:
		cubicinverseerp(newNumber, this->fromNumber, this->toNumber, this->currentTime, this->totalTime); break;
	case INTERPOLATION_TYPE_SINE:
		sineerp(newNumber, this->fromNumber, this->toNumber, this->currentTime, this->totalTime); break;
	case INTERPOLATION_TYPE_EXPONENTIAL:
		exponentialerp(newNumber, this->fromNumber, this->toNumber, this->currentTime, this->totalTime); break;
	case INTERPOLATION_TYPE_OVERSHOOT:
		overshooterp(newNumber, this->fromNumber, this->toNumber, this->currentTime, this->totalTime); break;
	case INTERPOLATION_TYPE_PARABOLA:
		ASSERT(0, "Valid interpolation type, %d -- _PARABOLA not implemented for pure number tweens", this->interpolationType); break;
	default:
		ASSERT(0, "Valid interpolation type, %d", this->interpolationType); break;
	}

	this->currentTime += deltaTime;
	if (this->currentTime > this->totalTime || this->continuousUpdates) {
		ASSERT(this->callback != 0, "Callback not 0");
		clamp(newNumber, this->fromNumber, this->toNumber);
		this->callback(this->fromNumber, this->toNumber, newNumber, this->tweenName, this->userParams);
	}

	if (this->currentTime >= this->totalTime) {
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

OnGoingNumberTweenDetails::~OnGoingNumberTweenDetails() {
	if (this->userParams != nullptr) {
		delete this->userParams;
	}
}


////////////////////////////////////////////////////////////////////////////////
