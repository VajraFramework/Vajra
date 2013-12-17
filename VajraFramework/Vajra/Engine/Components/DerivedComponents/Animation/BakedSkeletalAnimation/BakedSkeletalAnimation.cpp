#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/BakedSkeletalAnimation/BakedSkeletalAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/BakedSkeletalAnimationClip/BakedSkeletalAnimationClip.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

BakedSkeletalAnimation::BakedSkeletalAnimation() : Animation() {
	this->init();
}

BakedSkeletalAnimation::BakedSkeletalAnimation(Object* object_) : Animation(object_) {
	this->init();
}

BakedSkeletalAnimation::~BakedSkeletalAnimation() {
	this->destroy();
}

AnimationClip* BakedSkeletalAnimation::AddAnimationClip(std::string urlOfAnimationClip) {
	BakedSkeletalAnimationClip* newBakedSkeletalAnimationClip = new BakedSkeletalAnimationClip(this);
	newBakedSkeletalAnimationClip->InitAnimationClip(urlOfAnimationClip);

	return this->AddAnimationClip(newBakedSkeletalAnimationClip);
}

AnimationClip* BakedSkeletalAnimation::AddAnimationClip(AnimationClip* animationClip) {
	return this->addAnimationClip(animationClip, true);
}

void BakedSkeletalAnimation::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}
}

void BakedSkeletalAnimation::destroy() {
}

