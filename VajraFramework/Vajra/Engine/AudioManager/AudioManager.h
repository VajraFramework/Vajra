//
//  AudioManager.h
//  Created by Matt Kaufmann on 12/04/13.
//

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "Libraries/glm/glm.hpp"

class AudioManagerInternal;
class AudioPlayer;

class AudioManager {
public:
	~AudioManager();

	bool Is3DSoundEnabled();

	void Enable3DSound();
	void Disable3DSound();
	void SetListenerPosition(glm::vec3 pos);
	void SetListenerPosition(float x, float y, float z);
	void SetListenerOrientation(glm::quat orient);
	void SetListenerOrientation(glm::vec3 forward, glm::vec3 up);
	void SetListenerVelocity(glm::vec3 pos);
	void SetListenerVelocity(float x, float y, float z);
	void SetListenerVolume(float volume);

	AudioPlayer* RequestAudioPlayer();
	void ReturnAudioPlayer(AudioPlayer* player);
	//void ReleaseALSource(ALuint source);

	void PauseAllAudio();
	void ResumeAllAudio();
	void StopAllAudio();

private:
	AudioManager();

	void init();
	void destroy();

	AudioManagerInternal* internalMgr;

	friend class Engine;
};

#endif // AUDIOMANAGER_H
