#ifndef BAKED_SKELETAL_ANIMATION_H
#define BAKED_SKELETAL_ANIMATION_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/Animation.h"

#include <map>
#include <string>

// Forward Declarations:
class GameObject;
class BakedSkeletalAnimationClip;

class BakedSkeletalAnimation : public Animation, public Component {
public:
	BakedSkeletalAnimation();
	BakedSkeletalAnimation(Object* object_);
	virtual ~BakedSkeletalAnimation();

	static inline ComponentIdType GetTypeId() { return BakedSkeletalAnimation::componentTypeId; }

	// @Override
	void HandleMessage(MessageChunk messageChunk);

	virtual AnimationClip* AddAnimationClip(std::string urlOfAnimationClip);
	virtual AnimationClip* AddAnimationClip(AnimationClip* animationClip);

private:
	void init();
	void destroy();

	static unsigned int componentTypeId;
};

#endif // BAKED_SKELETAL_ANIMATION_H
