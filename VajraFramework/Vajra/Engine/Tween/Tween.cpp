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

void Tween::TweenPosition(ObjectIdType gameObjectId, glm::vec3 initialPosition, glm::vec3 finalPosition, float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		glm::quat currentOrientation = gameObject->GetTransform()->GetOrientation();
		glm::vec3 currentScale       = gameObject->GetTransform()->GetScale();
		Tween::tweenTransform_internal(gameObject, initialPosition, finalPosition,
												  currentOrientation, currentOrientation,
												  currentScale, currentScale,
												  time, callback);
	}
}

void Tween::TweenPosition(ObjectIdType gameObjectId, glm::vec3 finalPosition, float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		glm::quat currentOrientation = gameObject->GetTransform()->GetOrientation();
		glm::vec3 currentScale       = gameObject->GetTransform()->GetScale();
		Tween::tweenTransform_internal(gameObject, gameObject->GetTransform()->GetPosition(), finalPosition,
											  	  currentOrientation, currentOrientation,
											  	  currentScale, currentScale,
											  	  time, callback);
	}
}

void Tween::TweenOrientation(ObjectIdType gameObjectId, glm::quat initialOrientation, glm::quat finalOrientation, float time,
		void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName) /* = 0 */) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		glm::vec3 currentPosition    = gameObject->GetTransform()->GetPosition();
		glm::vec3 currentScale       = gameObject->GetTransform()->GetScale();
		Tween::tweenTransform_internal(gameObject, currentPosition, currentPosition,
											  	  initialOrientation, finalOrientation,
											  	  currentScale, currentScale,
											  	  time, callback);
	}
}

void Tween::TweenOrientation(ObjectIdType gameObjectId, glm::quat finalOrientation, float time,
		void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		glm::vec3 currentPosition    = gameObject->GetTransform()->GetPosition();
		glm::vec3 currentScale       = gameObject->GetTransform()->GetScale();
		Tween::tweenTransform_internal(gameObject, currentPosition, currentPosition,
											  	  gameObject->GetTransform()->GetOrientation(), finalOrientation,
											  	  currentScale, currentScale,
											  	  time, callback);
	}
}

void Tween::TweenScale(ObjectIdType gameObjectId, glm::vec3 initialScale, glm::vec3 finalScale, float time,
		void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		glm::vec3 currentPosition    = gameObject->GetTransform()->GetPosition();
		glm::quat currentOrientation = gameObject->GetTransform()->GetOrientation();
		Tween::tweenTransform_internal(gameObject, currentPosition, currentPosition,
											  	  currentOrientation, currentOrientation,
											  	  initialScale, finalScale,
											  	  time, callback);
	}
}

void Tween::TweenScale(ObjectIdType gameObjectId, glm::vec3 finalScale, float time,
		void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		glm::vec3 currentPosition    = gameObject->GetTransform()->GetPosition();
		glm::quat currentOrientation = gameObject->GetTransform()->GetOrientation();
		Tween::tweenTransform_internal(gameObject, currentPosition, currentPosition,
											  	  currentOrientation, currentOrientation,
											  	  gameObject->GetTransform()->GetScale(), finalScale,
											  	  time, callback);
	}
}

void Tween::TweenTransform(ObjectIdType gameObjectId, glm::vec3 initialPosition, glm::vec3 finalPosition,
												      glm::quat initialOrientation, glm::quat finalOrientation,
												      glm::vec3 initialScale, glm::vec3 finalScale,
												      float time,
												      void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		this->tweenTransform_internal(gameObject, initialPosition, finalPosition,
												  initialOrientation, finalOrientation,
												  initialScale, finalScale,
												  time, callback);
	}
}

void Tween::TweenToNumber(float fromNumber, float toNumber, float timePeriod, bool continuousUpdates, std::string tweenName,
						  void (*callback)(float normalizedProgress, std::string tweenName)) {
	OnGoingNumberTweenDetails* newNumberTween = new OnGoingNumberTweenDetails();
	newNumberTween->fromNumber                = fromNumber;
	newNumberTween->toNumber                  = toNumber;
	newNumberTween->totalTime                 = timePeriod;
	newNumberTween->callback                  = callback;
	newNumberTween->continuousUpdates         = continuousUpdates;
	newNumberTween->tweenName                 = tweenName;
	ASSERT(timePeriod != 0, "TimePeriod to tween over is not zero");

	this->ongoingNumberTweens.push_back(newNumberTween);
}

void Tween::updateTweens() {
	float deltaTime = ENGINE->GetTimer()->GetDeltaFrameTime();
	for (auto it = this->ongoingNumberTweens.begin(); it != this->ongoingNumberTweens.end(); ++it) {
		OnGoingNumberTweenDetails* tweenDetails = *it;

		float newCurrentNumber = tweenDetails->currentNumber + deltaTime * (tweenDetails->toNumber - tweenDetails->fromNumber) / tweenDetails->totalTime;
		float normalizedProgress = newCurrentNumber / (tweenDetails->toNumber - tweenDetails->fromNumber);
		if (normalizedProgress > 1.0f || tweenDetails->continuousUpdates) {
			tweenDetails->callback(normalizedProgress, tweenDetails->tweenName);
		}
		tweenDetails->currentNumber = newCurrentNumber;

		// Erase completed tweens:
		if (normalizedProgress > 1.0f) {
			std::list<OnGoingNumberTweenDetails*>::iterator nextIt = this->ongoingNumberTweens.erase(it);
			if (nextIt != this->ongoingNumberTweens.end()) {
				it = nextIt;
			} else {
				break;
			}
		}
	}
}

void Tween::tweenTransform_internal(GameObject* gameObject, glm::vec3 initialPosition, glm::vec3 finalPosition,
														glm::quat initialOrientation, glm::quat finalOrientation,
														glm::vec3 initialScale, glm::vec3 finalScale,
														float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	if (gameObject == nullptr) {
		FRAMEWORK->GetLogger()->dbglog("\nTrying to tween null GameObject");
		return;
	}

	if (this->IsTweening(gameObject->GetId())) {
		FRAMEWORK->GetLogger()->dbglog("\nWARNING GameObject %d is already tweening, ignoring tween command", gameObject->GetId());
		return;
	}


	RigidAnimation* rigidAnimation = gameObject->GetComponent<RigidAnimation>();
	if (rigidAnimation == nullptr) {
		rigidAnimation = gameObject->AddComponent<RigidAnimation>();
		FRAMEWORK->GetLogger()->dbglog("\nTween added RigidAnimation Component to GameObject %d", gameObject->GetId());
	}

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

	std::vector<AnimationKeyFrame*> keyframes;
	keyframes.push_back(initialKeyFrame);
	keyframes.push_back(finalKeyFrame);

	std::string tweenClipName = getRandomTweenName();
	//
	OnGoingTransformTweenDetails* ongoingTweenDetails = new OnGoingTransformTweenDetails();
	ongoingTweenDetails->tweenClipName = tweenClipName;
	ongoingTweenDetails->callback = callback;
	this->ongoingTransformTweens[gameObject->GetId()] = ongoingTweenDetails;

	RigidAnimationClip* newAnimationClip = new RigidAnimationClip(rigidAnimation);
	newAnimationClip->InitAnimationClip(tweenClipName, keyframes);
	newAnimationClip->SetLooping(false);
	newAnimationClip->SetIsTween(true);

	rigidAnimation->AddAnimationClip(newAnimationClip);

	rigidAnimation->PlayAnimationClip(tweenClipName);
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

