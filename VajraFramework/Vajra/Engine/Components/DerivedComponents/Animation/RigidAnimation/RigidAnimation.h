#ifndef RIGID_ANIMATION_H
#define RIGID_ANIMATION_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/Animation.h"

#include <map>
#include <string>

// Forward Declarations:
class GameObject;
class RigidAnimationClip;

class RigidAnimation : public Animation {
public:
	RigidAnimation();
	RigidAnimation(Object* object_);
	virtual ~RigidAnimation();

	static inline ComponentIdType GetTypeId() { return Animation::GetTypeId(); }

	virtual AnimationClip* AddAnimationClip(std::string urlOfAnimationClip);

private:
	void init();
	void destroy();
};

#endif // RIGID_ANIMATION_H
