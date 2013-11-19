#ifndef ANIMATION_CLIP_H
#define ANIMATION_CLIP_H

#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"

#include <memory>
#include <string>

// Forward Declarations:
class Animation;
class AnimationKeyFrame;
class Transform;

class AnimationClip {
public:
	AnimationClip(Animation* parentAnimationComponent_);
	virtual ~AnimationClip();

	inline std::string GetName()    const { return this->clipName; }
	inline void SetName(std::string name) { this->clipName = name; }

	inline bool IsPlaying()               const { return this->isPlaying; }
	inline bool IsLooping()               const { return this->isLooping; }
	inline bool IsTween()                 const { return this->isTween;   }
	inline void SetLooping(bool looping)   { this->isLooping = looping;   }
	inline void SetIsTween(bool isTween_)  { this->isTween   = isTween_;  }

	inline double GetPlaybackSpeed() const { return this->playbackSpeed;  }
	inline void SetPlaybackSpeed(double speed) { this->playbackSpeed = speed; }

protected:
	float interpolation;
	virtual AnimationKeyFrame* getCurrentKeyFrame() const = 0;
	virtual AnimationKeyFrame* getNextKeyFrame() const = 0;
	virtual AnimationKeyFrame* getCurrentKeyFrameAtInterpolation(float interpolation) = 0;
	virtual unsigned int getCurrentKeyFrameIndex() const = 0;
	virtual void setCurrentKeyFrameIndex(unsigned int index) = 0;
	virtual unsigned int getNumKeyFrames() const = 0;

	virtual void InitAnimationClip(std::string urlOfAnimationClip) = 0;
	virtual void InitAnimationClip(std::string clipName_, std::vector<AnimationKeyFrame*> animationKeyFrames) = 0;

private:
	void init(Animation* parentAnimationComponent_);
	void destroy();

	void Play();
	void Pause();
	void Resume();
	void Stop();

	void step(double deltaTime);
	void apply(Transform* transform);

	// Utility Functions:
	void raiseEvent(const Message* const message);

	bool isPlaying;
	bool isLooping;

	bool isTween;

	double playbackSpeed;

	std::string clipName;

	Animation* parentAnimationComponent;

	// Friended to Animation so as to not have to expose step(), play(), etc publicly
	friend class Animation;
};

#endif // ANIMATION_CLIP_H
