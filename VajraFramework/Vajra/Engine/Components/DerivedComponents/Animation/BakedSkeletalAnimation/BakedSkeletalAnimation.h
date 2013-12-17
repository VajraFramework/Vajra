#ifndef BAKED_SKELETAL_ANIMATION_H
#define BAKED_SKELETAL_ANIMATION_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/Animation.h"

#include <map>
#include <string>

// Forward Declarations:
class GameObject;
class BakedSkeletalAnimationClip;

class BakedSkeletalAnimation : public Animation {
public:
	BakedSkeletalAnimation();
	BakedSkeletalAnimation(Object* object_);
	virtual ~BakedSkeletalAnimation();

	static inline ComponentIdType GetTypeId() { return Animation::GetTypeId(); }

	virtual AnimationClip* AddAnimationClip(std::string urlOfAnimationClip);
	virtual AnimationClip* AddAnimationClip(AnimationClip* animationClip);

private:
	void init();
	void destroy();
};

#endif // BAKED_SKELETAL_ANIMATION_H
