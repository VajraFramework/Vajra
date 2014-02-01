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
#include "Vajra/Engine/Tween/TweenCallbackComponent.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

#include <cstdio>

// Forward Declarations:
std::string getRandomTweenName();

Tween::Tween() {
}

Tween::~Tween() {
	this->destroy();
}

void Tween::TweenPosition(ObjectIdType gameObjectId, glm::vec3 initialPosition, glm::vec3 finalPosition, float time, TweenTranslationCurveType curveType /* = TWEEN_TRANSLATION_CURVE_TYPE_LINEAR */, bool looping /* = false */, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		glm::quat currentOrientation = gameObject->GetTransform()->GetOrientation();
		glm::vec3 currentScale       = gameObject->GetTransform()->GetScale();
		Tween::tweenTransform_internal(gameObject, initialPosition, finalPosition,
												  currentOrientation, currentOrientation,
												  currentScale, currentScale,
												  time, curveType, looping, callback);
	}
}

void Tween::TweenPosition(ObjectIdType gameObjectId, glm::vec3 finalPosition, float time, TweenTranslationCurveType curveType /* = TWEEN_TRANSLATION_CURVE_TYPE_LINEAR */, bool looping /* = false */, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		glm::quat currentOrientation = gameObject->GetTransform()->GetOrientation();
		glm::vec3 currentScale       = gameObject->GetTransform()->GetScale();
		Tween::tweenTransform_internal(gameObject, gameObject->GetTransform()->GetPosition(), finalPosition,
											  	  currentOrientation, currentOrientation,
											  	  currentScale, currentScale,
											  	  time, curveType, looping, callback);
	}
}

void Tween::TweenOrientation(ObjectIdType gameObjectId, glm::quat initialOrientation, glm::quat finalOrientation, float time, bool looping /* = false */,
		void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) /* = 0 */) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		glm::vec3 currentPosition    = gameObject->GetTransform()->GetPosition();
		glm::vec3 currentScale       = gameObject->GetTransform()->GetScale();
		Tween::tweenTransform_internal(gameObject, currentPosition, currentPosition,
											  	  initialOrientation, finalOrientation,
											  	  currentScale, currentScale,
											  	  time, TWEEN_TRANSLATION_CURVE_TYPE_LINEAR, looping, callback);
	}
}

void Tween::TweenOrientation(ObjectIdType gameObjectId, glm::quat finalOrientation, float time, bool looping /* = false */,
		void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		glm::vec3 currentPosition    = gameObject->GetTransform()->GetPosition();
		glm::vec3 currentScale       = gameObject->GetTransform()->GetScale();
		Tween::tweenTransform_internal(gameObject, currentPosition, currentPosition,
											  	  gameObject->GetTransform()->GetOrientation(), finalOrientation,
											  	  currentScale, currentScale,
											  	  time, TWEEN_TRANSLATION_CURVE_TYPE_LINEAR, looping, callback);
	}
}

void Tween::TweenScale(ObjectIdType gameObjectId, glm::vec3 initialScale, glm::vec3 finalScale, float time, bool looping /* = false */,
		void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		glm::vec3 currentPosition    = gameObject->GetTransform()->GetPosition();
		glm::quat currentOrientation = gameObject->GetTransform()->GetOrientation();
		Tween::tweenTransform_internal(gameObject, currentPosition, currentPosition,
											  	  currentOrientation, currentOrientation,
											  	  initialScale, finalScale,
											  	  time, TWEEN_TRANSLATION_CURVE_TYPE_LINEAR, looping, callback);
	}
}

void Tween::TweenScale(ObjectIdType gameObjectId, glm::vec3 finalScale, float time, bool looping /* = false */,
		void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		glm::vec3 currentPosition    = gameObject->GetTransform()->GetPosition();
		glm::quat currentOrientation = gameObject->GetTransform()->GetOrientation();
		Tween::tweenTransform_internal(gameObject, currentPosition, currentPosition,
											  	  currentOrientation, currentOrientation,
											  	  gameObject->GetTransform()->GetScale(), finalScale,
											  	  time, TWEEN_TRANSLATION_CURVE_TYPE_LINEAR, looping, callback);
	}
}

void Tween::TweenTransform(ObjectIdType gameObjectId, glm::vec3 initialPosition, glm::vec3 finalPosition,
												      glm::quat initialOrientation, glm::quat finalOrientation,
												      glm::vec3 initialScale, glm::vec3 finalScale,
												      float time, TweenTranslationCurveType curveType /* = TWEEN_TRANSLATION_CURVE_TYPE_LINEAR */, bool looping /* = false */,
												      void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		this->tweenTransform_internal(gameObject, initialPosition, finalPosition,
												  initialOrientation, finalOrientation,
												  initialScale, finalScale,
												  time, curveType, looping, callback);
	}
}

void Tween::TweenToNumber(float fromNumber, float toNumber, float timePeriod, bool looping, bool continuousUpdates, std::string tweenName,
						  void (*callback)(float fromNumber, float toNumber, float currentNumber, std::string tweenName)) {
	OnGoingNumberTweenDetails* newNumberTween = new OnGoingNumberTweenDetails();
	newNumberTween->fromNumber                = fromNumber;
	newNumberTween->currentNumber             = fromNumber;
	newNumberTween->toNumber                  = toNumber;
	newNumberTween->totalTime                 = timePeriod;
	newNumberTween->callback                  = callback;
	newNumberTween->continuousUpdates         = continuousUpdates;
	newNumberTween->tweenName                 = tweenName;
	newNumberTween->looping                   = looping;
	ASSERT(timePeriod != 0, "TimePeriod to tween over is not zero");

	this->ongoingNumberTweens.push_back(newNumberTween);
}

void Tween::updateTweens() {
	float deltaTime = ENGINE->GetTimer()->GetDeltaFrameTime();
	for (auto it = this->ongoingNumberTweens.begin(); it != this->ongoingNumberTweens.end(); ++it) {
		OnGoingNumberTweenDetails* tweenDetails = *it;

		float newCurrentNumber = tweenDetails->currentNumber + deltaTime * (tweenDetails->toNumber - tweenDetails->fromNumber) / tweenDetails->totalTime;
		tweenDetails->currentNumber = newCurrentNumber;
		if (newCurrentNumber > tweenDetails->toNumber || tweenDetails->continuousUpdates) {
			tweenDetails->callback(tweenDetails->fromNumber, tweenDetails->toNumber, newCurrentNumber, tweenDetails->tweenName);
		}

		if (newCurrentNumber > tweenDetails->toNumber) {
			if (tweenDetails->looping) {
				tweenDetails->currentNumber = tweenDetails->fromNumber;
			} else {
				// Erase completed tweens:
				std::list<OnGoingNumberTweenDetails*>::iterator nextIt = this->ongoingNumberTweens.erase(it);
				if (nextIt != this->ongoingNumberTweens.end()) {
					it = nextIt;
				} else {
					break;
				}
			}
		}
	}
}

void Tween::tweenTransform_internal(GameObject* gameObject, glm::vec3 initialPosition, glm::vec3 finalPosition,
														glm::quat initialOrientation, glm::quat finalOrientation,
														glm::vec3 initialScale, glm::vec3 finalScale,
														float time, TweenTranslationCurveType curveType, bool looping, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	if (gameObject == nullptr) {
		FRAMEWORK->GetLogger()->dbglog("\nTrying to tween null GameObject");
		return;
	}

	if (this->IsTweening(gameObject->GetId())) {
		FRAMEWORK->GetLogger()->dbglog("\nWARNING GameObject %d is already tweening, ignoring tween command", gameObject->GetId());
		return;
	}

	// Reset the current transform of the game object to the origin:
	gameObject->GetTransform()->SetPosition(ZERO_VEC3);
	gameObject->GetTransform()->SetOrientation(IDENTITY_QUATERNION);
	gameObject->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);

	RigidAnimation* rigidAnimation = gameObject->GetComponent<RigidAnimation>();
	if (rigidAnimation == nullptr) {
		rigidAnimation = gameObject->AddComponent<RigidAnimation>();
		FRAMEWORK->GetLogger()->dbglog("\nTween added RigidAnimation Component to GameObject %d", gameObject->GetId());
	}

	std::string tweenClipName = getRandomTweenName();
	//
	OnGoingTransformTweenDetails* ongoingTweenDetails = new OnGoingTransformTweenDetails();
	ongoingTweenDetails->tweenClipName = tweenClipName;
	ongoingTweenDetails->callback = callback;
	ongoingTweenDetails->looping = looping;
	this->ongoingTransformTweens[gameObject->GetId()] = ongoingTweenDetails;

	RigidAnimationClip* newAnimationClip = new RigidAnimationClip(rigidAnimation);
	std::vector<AnimationKeyFrame*> keyframes;
	this->populateRigidAnimationKeyframesForTweenTransform(&keyframes,
							initialPosition, finalPosition,
							initialOrientation, finalOrientation,
							initialScale, finalScale,
							time,
							curveType);
	newAnimationClip->InitAnimationClip(tweenClipName, keyframes);
	newAnimationClip->SetLooping(looping);
	newAnimationClip->SetIsTween(true);


	rigidAnimation->AddAnimationClip(newAnimationClip);

	rigidAnimation->PlayAnimationClip(tweenClipName);
}

#define TWEEN_PARABOLA_a 0.1f                  // From y = 4ax^2
#define TWEEN_PARABOLA_NUM_SAMPLES 10

void Tween::populateRigidAnimationKeyframesForTweenTransform(std::vector<AnimationKeyFrame*>* keyframes,
															 glm::vec3& initialPosition, glm::vec3& finalPosition,
															 glm::quat& initialOrientation, glm::quat& finalOrientation,
															 glm::vec3& initialScale, glm::vec3& finalScale,
															 float time,
															 TweenTranslationCurveType curveType) {

	switch (curveType) {
	case TWEEN_TRANSLATION_CURVE_TYPE_LINEAR: {
		RigidAnimationKeyFrame* initialKeyFrame = new RigidAnimationKeyFrame();
		RigidAnimationKeyFrame* finalKeyFrame   = new RigidAnimationKeyFrame();
		//
		initialKeyFrame->SetTime(0.0f);
		initialKeyFrame->SetTranslation(initialPosition);
		initialKeyFrame->SetRotation(initialOrientation);
		initialKeyFrame->SetScaling(initialScale);
		//
		finalKeyFrame->SetTime(time);
		finalKeyFrame->SetTranslation(finalPosition);
		finalKeyFrame->SetRotation(finalOrientation);
		finalKeyFrame->SetScaling(finalScale);

		keyframes->push_back(initialKeyFrame);
		keyframes->push_back(finalKeyFrame);


	} break;

	case TWEEN_TRANSLATION_CURVE_TYPE_PARABOLA: {
		/*
		 * This assumes that the required parabola is one in the plane that is perpendicular to the XZ plane
		 */
		// TODO [Implement] Parabolas in arbitrary planes:

		float parabolaBaseLength = glm::distance(initialPosition, finalPosition);

		for (int sample = 0; sample < TWEEN_PARABOLA_NUM_SAMPLES; ++sample) {
			float normalizedSampleNumber = ((float)sample / (float)TWEEN_PARABOLA_NUM_SAMPLES);

			RigidAnimationKeyFrame* keyframe = new RigidAnimationKeyFrame();
			keyframe->SetTime(time * normalizedSampleNumber);

			float maxHeightOfParabola = 0.5f * std::sqrt(parabolaBaseLength / 2.0f / TWEEN_PARABOLA_a);

			glm::vec3 positionAlongParabola;
			lerp(positionAlongParabola.x, initialPosition.x, finalPosition.x, normalizedSampleNumber);
			lerp(positionAlongParabola.z, initialPosition.z, finalPosition.z, normalizedSampleNumber);
			float parabola_y = (parabolaBaseLength * normalizedSampleNumber) - (parabolaBaseLength / 2.0f);
			positionAlongParabola.y = initialPosition.y +
									  maxHeightOfParabola -
									  (0.5f *
									  std::sqrt(std::abs(parabola_y / TWEEN_PARABOLA_a)));

			glm::quat slerpedOrientation;
			slerp(slerpedOrientation, initialOrientation, finalOrientation, normalizedSampleNumber);

			glm::vec3 lerpedScaling;
			lerp(lerpedScaling, initialScale, finalScale, normalizedSampleNumber);

			keyframe->SetTranslation(positionAlongParabola);
			keyframe->SetRotation(slerpedOrientation);
			keyframe->SetScaling(lerpedScaling);

			keyframes->push_back(keyframe);
		}
		//
		RigidAnimationKeyFrame* finalKeyFrame   = new RigidAnimationKeyFrame();
		finalKeyFrame->SetTime(time);
		finalKeyFrame->SetTranslation(finalPosition);
		finalKeyFrame->SetRotation(finalOrientation);
		finalKeyFrame->SetScaling(finalScale);
		//
		// keyframes->push_back(finalKeyFrame);

	} break;

	default: ASSERT(0, "Recognized curve type: %d", curveType);
	}
}

bool Tween::IsTweening(ObjectIdType gameObjectId) {
	return (this->ongoingTransformTweens.find(gameObjectId) != this->ongoingTransformTweens.end());
}

OnGoingTransformTweenDetails* Tween::getOnGoingTransformTweenDetails(ObjectIdType gameObjectId) {
	auto ongoingTweenIt = this->ongoingTransformTweens.find(gameObjectId);
	if (ongoingTweenIt != this->ongoingTransformTweens.end()) {
		return ongoingTweenIt->second;
	}
	return nullptr;
}

void Tween::init() {
	this->AddComponent<TweenCallbackComponent>();

}

void Tween::destroy() {
}

////////////////////////////////////////////////////////////////////////////////

std::string getRandomTweenName() {
	// Come up with a temporary name for the tween animation:
	static unsigned long int nonce = 0;
	nonce++;
	char buffer[20];
	sprintf(buffer, "%lu", nonce);

	std::string tweenClipName = "tween";
	tweenClipName = tweenClipName + buffer;

	return tweenClipName;
}


////////////////////////////////////////////////////////////////////////////////

