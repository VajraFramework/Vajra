#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/RigidAnimation/RigidAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/RigidAnimationClip/RigidAnimationClip.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

RigidAnimation::RigidAnimation() : Animation() {
	this->init();
}

RigidAnimation::RigidAnimation(Object* object_) : Animation(object_) {
	this->init();
}

RigidAnimation::~RigidAnimation() {
	this->destroy();
}

AnimationClip* RigidAnimation::AddAnimationClip(std::string urlOfAnimationClip) {
	RigidAnimationClip* newRigidAnimationClip = new RigidAnimationClip(this);
	newRigidAnimationClip->InitAnimationClip(urlOfAnimationClip);

	return this->AddAnimationClip(newRigidAnimationClip);
}

AnimationClip* RigidAnimation::AddAnimationClip(AnimationClip* animationClip) {
	return this->addAnimationClip(animationClip, true);
}

void RigidAnimation::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}
}

void RigidAnimation::destroy() {
}

