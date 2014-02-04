#ifndef BAKED_SKELETAL_ANIMATION_H
#define BAKED_SKELETAL_ANIMATION_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/Animation.h"

#include <map>
#include <string>

// Forward Declarations:
class GameObject;
class BakedSkeletalAnimationClip;

//[[COMPONENT]]//
class BakedSkeletalAnimation : public Animation, public Component {
public:
	BakedSkeletalAnimation();
	BakedSkeletalAnimation(Object* object_);
	virtual ~BakedSkeletalAnimation();

	static inline ComponentIdType GetTypeId() { return BakedSkeletalAnimation::componentTypeId; }

	// @Override
	void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	virtual AnimationClip* AddAnimationClip(std::string urlOfAnimationClip);
	virtual AnimationClip* AddAnimationClip(AnimationClip* animationClip);

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
void BakedSkeletalAnimation::PlayAnimationClip(std::string animationClipName) { this->playAnimationClip(animationClipName); }
void BakedSkeletalAnimation::PlayAnimationClip()   { this->playAnimationClip();   }
void BakedSkeletalAnimation::PauseAnimationClip()  { this->pauseAnimationClip();  }
void BakedSkeletalAnimation::ResumeAnimationClip() { this->resumeAnimationClip(); }
void BakedSkeletalAnimation::StopAnimationClip()   { this->stopAnimationClip();   }

#endif // BAKED_SKELETAL_ANIMATION_H
