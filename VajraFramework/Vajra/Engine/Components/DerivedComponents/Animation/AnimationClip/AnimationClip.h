#ifndef ANIMATION_CLIP_H
#define ANIMATION_CLIP_H

#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationClipDataAsset.h"

#include <memory>
#include <string>

class AnimationClip {
public:
	AnimationClip();
	virtual ~AnimationClip();

	virtual void InitAnimationClip(std::string urlOfAnimationClip) = 0;

	inline std::string GetName()    const { return this->clipName; }
	inline void SetName(std::string name) { this->clipName = name; }

	void Play();
	void Pause();
	void Resume();
	void Stop();

	inline bool IsPlaying()               const { return this->isPlaying; }
	inline bool IsLooping()               const { return this->isLooping; }
	inline void SetLooping(bool looping)  { this->isLooping = looping; }

	inline float GetPlaybackSpeed() const { return this->playbackSpeed; }
	inline void SetPlaybackSpeed(float speed) { this->playbackSpeed = speed; }

private:
	void init();
	void destroy();

	bool isPlaying;
	bool isLooping;

	float playbackSpeed;

	std::string clipName;

	// This should be in the derived class
#if 0
	std::shared_ptr<AnimationClipDataAsset> clipDataAsset;
#endif
};

#endif // ANIMATION_CLIP_H
