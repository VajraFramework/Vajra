#ifndef ANIMATION_CLIP_H
#define ANIMATION_CLIP_H

#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"

#include <memory>
#include <string>

// Forward Declarations:
class AnimationKeyFrame;
class Transform;

class AnimationClip {
public:
	AnimationClip();
	virtual ~AnimationClip();

	inline std::string GetName()    const { return this->clipName; }
	inline void SetName(std::string name) { this->clipName = name; }

	inline bool IsPlaying()               const { return this->isPlaying; }
	inline bool IsLooping()               const { return this->isLooping; }
	inline void SetLooping(bool looping)  { this->isLooping = looping; }

	inline double GetPlaybackSpeed() const { return this->playbackSpeed; }
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

private:
	void init();
	void destroy();

	void Play();
	void Pause();
	void Resume();
	void Stop();

	void step(double deltaTime);
	void apply(Transform* transform);

	bool isPlaying;
	bool isLooping;

	double playbackSpeed;

	std::string clipName;

	// Friended to Animation so as to not have to expose step(), play(), etc publicly
	friend class Animation;
};

#endif // ANIMATION_CLIP_H
