#ifndef ANIMATION_H
#define ANIMATION_H

#include "Vajra/Common/Components/Component.h"

#include <map>
#include <string>

// Forward Declarations:
class GameObject;
class AnimationClip;

class Animation {
public:
	Animation();
	Animation(GameObject* gameObject_);
	virtual ~Animation();

	virtual AnimationClip* AddAnimationClip(std::string urlOfAnimationClip) = 0;
	virtual AnimationClip* AddAnimationClip(AnimationClip* animationClip) = 0;
	void DeleteAnimationClip(std::string animationClipName);

	bool IsPlaying();
	bool IsPlaying(std::string animationClipName);

	AnimationClip* GetAnimationClip(std::string animationClipName);
	inline AnimationClip* GetCurrentPlayingAnimationClip() { return this->currentAnimationClip; }

	inline void Bind() { this->bind(); }

	inline GameObject* GetGameObject() { return this->gameObject; }

protected:
	bool handleMessage(MessageChunk messageChunk);
	AnimationClip* addAnimationClip(AnimationClip* animationClip, bool takeOwnershipOfMemory);

	void playAnimationClip(std::string animationClipName);
	void playAnimationClip();
	void pauseAnimationClip();
	void resumeAnimationClip();
	void stopAnimationClip();

private:
	void init(GameObject* gameObject_);
	void destroy();

	// Utility functions:
	void playAnimationClip_internal(AnimationClip* animationClip);
	void step(double deltaTime);
	void apply();
	void bind();

	AnimationClip* currentAnimationClip;
	std::map<std::string /* clip name */, AnimationClip*> animationClips;

	GameObject* gameObject;
};

#endif // ANIMATION_H
