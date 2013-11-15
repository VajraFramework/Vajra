#ifndef ANIMATION_H
#define ANIMATION_H

#include "Vajra/Common/Components/Component.h"

#include <map>
#include <string>

// Forward Declarations:
class GameObject;
class AnimationClip;

class Animation : public Component {
public:
	Animation();
	Animation(Object* object_);
	virtual ~Animation();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(Message* message);

	virtual void AddAnimationClip(std::string urlOfAnimationClip) = 0;
	void DeleteAnimationClip(std::string animationClipName);

	void PlayAnimationClip(std::string animationClipName);
	void PlayAnimationClip();
	void PauseAnimationClip();
	void ResumeAnimationClip();
	void StopAnimationClip();

	bool IsPlaying();
	bool IsPlaying(std::string animationClipName);

	inline AnimationClip* GetCurrentPlayingAnimationClip() { return this->currentAnimationClip; }

protected:
	void addAnimationClip(AnimationClip* animationClip, bool takeOwnershipOfMemory);

private:
	void init();
	void destroy();

	// Utility functions:
	void playAnimationClip_internal(AnimationClip* animationClip);

	AnimationClip* currentAnimationClip;
	std::map<std::string /* clip name */, AnimationClip*> animationClips;

	static unsigned int componentTypeId;
};

#endif // ANIMATION_H
