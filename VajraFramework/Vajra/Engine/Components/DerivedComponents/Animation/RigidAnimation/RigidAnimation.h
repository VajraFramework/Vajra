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

	//[[PROPERTY]]//
	inline void PlayAnimationClip(std::string animationClipName);
	inline void PlayAnimationClip();
	inline void PauseAnimationClip();
	inline void ResumeAnimationClip();
	inline void StopAnimationClip();

private:
	void init();
	void destroy();

	static unsigned int componentTypeId;
};

////////////////////////////////////////////////////////////////////////////////

// Inline Functions:
void RigidAnimation::PlayAnimationClip(std::string animationClipName) { this->playAnimationClip(animationClipName); }
void RigidAnimation::PlayAnimationClip()   { this->playAnimationClip();   }
void RigidAnimation::PauseAnimationClip()  { this->pauseAnimationClip();  }
void RigidAnimation::ResumeAnimationClip() { this->resumeAnimationClip(); }
void RigidAnimation::StopAnimationClip()   { this->stopAnimationClip();   }

#endif // RIGID_ANIMATION_H
