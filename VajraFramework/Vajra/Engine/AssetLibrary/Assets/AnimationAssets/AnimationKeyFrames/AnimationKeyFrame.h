#ifndef ANIMATION_KEYFRAME_H
#define ANIMATION_KEYFRAME_H

#include <memory>
#include <string>
#include <vector>

// Forward Declarations:
class Transform;

class AnimationKeyFrame {
public:
	AnimationKeyFrame();
	virtual ~AnimationKeyFrame();

	inline float GetTime() const { return this->time; }
	inline void SetTime(float time_) { this->time = time_; }

	virtual void ApplyKeyFrameToTransform(Transform* transform) const = 0;

private:
	void init();
	void destroy();

	float time;
};

#endif // ANIMATION_KEYFRAME_H
