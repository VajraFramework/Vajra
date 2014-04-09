//
//  AudioManager.h
//  Created by Matt Kaufmann on 12/04/13.
//

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#ifdef PLATFORM_IOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include "Libraries/openal/headers/al.h"
#include "Libraries/openal/headers/alc.h"
#endif

#include <vector>

#define MAXIMUM_AUDIO_SOURCES 32
#define SOURCE_CHUNK_SIZE 8

class AudioManager {
public:
	~AudioManager();

	ALuint RequestALSource();
	void ReleaseALSource(ALuint source);

	void PauseAllAudio();
	void ResumeAllAudio();
	void StopAllAudio();

private:
	AudioManager();

	void init();
	void destroy();

	void generateMoreSources();

	ALCdevice* device;
	ALCcontext* context;

	ALuint sources[MAXIMUM_AUDIO_SOURCES];
	std::vector<ALuint> availableSources;
	int nSources;

	friend class Engine;
};

#endif // AUDIOMANAGER_H
