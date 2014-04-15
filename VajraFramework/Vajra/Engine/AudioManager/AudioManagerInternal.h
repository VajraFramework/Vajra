//
//	AudioManagerInternal.h
//	Created by Matt Kaufmann on 04/15/14.
//

#ifndef AUDIOMANAGERINTERNAL_H
#define AUDIOMANAGERINTERNAL_H

#include "Libraries/glm/glm.hpp"
#include "Vajra/Engine/AudioManager/AudioManagerInternal.h"

#ifdef PLATFORM_IOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include "Libraries/openal/headers/al.h"
#include "Libraries/openal/headers/alc.h"
#endif

#include <list>

#define MAXIMUM_AUDIO_SOURCES 256
#define SOURCE_CHUNK_SIZE 8

class AudioPlayer;

class AudioManagerInternal {
public:
	~AudioManagerInternal();

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
	//ALuint RequestALSource();
	//void ReleaseALSource(ALuint source);

	void PauseAllAudio();
	void ResumeAllAudio();
	void StopAllAudio();

private:
	AudioManagerInternal();

	void init();
	void destroy();

	void generateMoreSources();

	ALCdevice* device;
	ALCcontext* context;

	ALuint sources[MAXIMUM_AUDIO_SOURCES];
	//std::vector<ALuint> availableSources;
	std::list<AudioPlayer*> availablePlayers;
	std::list<AudioPlayer*> playersInUse;
	int nSources;

	friend class AudioManager;
};

#endif // AUDIOMANAGERINTERNAL_H
