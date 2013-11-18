#ifndef RIGID_ANIMATION_KEY_FRAME_H
#define RIGID_ANIMATION_KEY_FRAME_H

#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/AnimationKeyFrame.h"
#include "Vajra/Utilities/MathUtilities.h"

#include "Libraries/glm/glm.hpp"

// Forward Declarations:
class Transform;

class RigidAnimationKeyFrame : public AnimationKeyFrame {
public:
	RigidAnimationKeyFrame();
	virtual ~RigidAnimationKeyFrame();

	inline glm::vec3 GetTranslation() const { return this->translation; }
	inline glm::quat GetRotation()    const { return this->orientationQuat; }
	inline glm::vec3 GetScaling()     const { return this->scaling; }

	void SetTranslation(glm::vec3 v);
	void SetRotation(glm::quat q);
	void SetScaling(glm::vec3 v);

	// @Override
	virtual void ApplyKeyFrameToTransform(Transform* transform) const;

private:
	void init();
	void destroy();

	glm::vec3 translation;
	glm::quat orientationQuat;
	glm::vec3 scaling;
};


#endif // RIGID_ANIMATION_KEY_FRAME_H
