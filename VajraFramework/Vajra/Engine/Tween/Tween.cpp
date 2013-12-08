#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/RigidAnimationKeyFrame/RigidAnimationKeyFrame.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/RigidAnimationClip/RigidAnimationClip.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/RigidAnimation/RigidAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Engine/Tween/TweenCallbackComponent.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

Tween::Tween() {
}

Tween::~Tween() {
	this->destroy();
}

void Tween::TweenPosition(ObjectIdType gameObjectId, glm::vec3 initialPosition, glm::vec3 finalPosition, float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph()->GetGameObjectById(gameObjectId);
	Tween::tweenPosition_internal(gameObject, initialPosition, finalPosition, time, callback);
}

void Tween::TweenPosition(ObjectIdType gameObjectId, glm::vec3 finalPosition, float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
	GameObject* gameObject = ENGINE->GetSceneGraph()->GetGameObjectById(gameObjectId);
	if (gameObject != nullptr) {
		Tween::tweenPosition_internal(gameObject, gameObject->GetTransform()->GetPosition(), finalPosition, time, callback);
	}
}

void Tween::tweenPosition_internal(GameObject* gameObject, glm::vec3 initialPosition, glm::vec3 finalPosition, float time, void (*callback)(ObjectIdType gameObjectId, std::string tweenClipName)) {
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
	initialKeyFrame->SetRotation(gameObject->GetTransform()->GetOrientation());
	initialKeyFrame->SetScaling(gameObject->GetTransform()->GetScale());
	//
	finalKeyFrame->SetTime(time);
	finalKeyFrame->SetTranslation(finalPosition);
	finalKeyFrame->SetRotation(gameObject->GetTransform()->GetOrientation());
	finalKeyFrame->SetScaling(gameObject->GetTransform()->GetScale());

	std::vector<AnimationKeyFrame*> keyframes;
	keyframes.push_back(initialKeyFrame);
	keyframes.push_back(finalKeyFrame);

	// Come up with a temporary name for the tween animation:
	static unsigned long int nonce = 0;
	nonce++;
	char buffer[20];
	sprintf(buffer, "%lu", nonce);
	std::string tweenClipName = "tween";
	tweenClipName = tweenClipName + buffer;
	//
	OnGoingTweenDetails* ongoingTweenDetails = new OnGoingTweenDetails();
	ongoingTweenDetails->tweenClipName = tweenClipName;
	ongoingTweenDetails->callback = callback;
	this->ongoingTweens[gameObject->GetId()] = ongoingTweenDetails;

	RigidAnimationClip* newAnimationClip = new RigidAnimationClip(rigidAnimation);
	newAnimationClip->InitAnimationClip(tweenClipName, keyframes);
	newAnimationClip->SetLooping(false);
	newAnimationClip->SetIsTween(true);

	rigidAnimation->AddAnimationClip(newAnimationClip);

	rigidAnimation->PlayAnimationClip(tweenClipName);
}

bool Tween::IsTweening(ObjectIdType gameObjectId) {
	return (this->ongoingTweens.find(gameObjectId) != this->ongoingTweens.end());
}

OnGoingTweenDetails* Tween::getOnGoingTweenDetails(ObjectIdType gameObjectId) {
	auto ongoingTweenIt = this->ongoingTweens.find(gameObjectId);
	if (ongoingTweenIt != this->ongoingTweens.end()) {
		return ongoingTweenIt->second;
	}
	return nullptr;
}

void Tween::init() {
	this->AddComponent<TweenCallbackComponent>();

}

void Tween::destroy() {
}
