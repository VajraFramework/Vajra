#ifndef RIGID_ANIMATION_H
#define RIGID_ANIMATION_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/Animation.h"

#include <map>
#include <string>

// Forward Declarations:
class GameObject;
class RigidAnimationClip;

//[[COMPONENT]]//
class RigidAnimation : public Animation, public Component {
public:
	RigidAnimation();
	RigidAnimation(Object* object_);
	virtual ~RigidAnimation();

	static inline ComponentIdType GetTypeId() { return RigidAnimation::componentTypeId; }

	// @Override
	void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	virtual AnimationClip* AddAnimationClip(std::string urlOfAnimationClip);
	virtual AnimationClip* AddAnimationClip(AnimationClip* animationClip);

	// @Override
	virtual void Bind();

private:
	void init();
	void destroy();

	static unsigned int componentTypeId;
};

#endif // RIGID_ANIMATION_H
