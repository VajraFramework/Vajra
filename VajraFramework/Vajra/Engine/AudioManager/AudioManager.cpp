//
//  AudioManager.cpp
//  Created by Matt Kaufmann on 12/04/13.
//

#include "Vajra/Engine/AudioManager/AudioManager.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

#include <algorithm>

AudioManager::AudioManager() {
}

AudioManager::~AudioManager() {
	this->destroy();
}

void AudioManager::init() {
	// Open the audio device
	this->device = alcOpenDevice(nullptr);
	if (device == nullptr) {
		FRAMEWORK->GetLogger()->dbglog("\nCould not open audio device");
		return;
	}

	// Create and initialize the audio context
	this->context = alcCreateContext(this->device, nullptr);
	if (!alcMakeContextCurrent(this->context)) {
		FRAMEWORK->GetLogger()->dbglog("\nCould not create audio context");
		return;
	}

	// Set the default listener for OpenAL.
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f}; // Listener forward and up vectors
	alListenerfv(AL_ORIENTATION, listenerOri);

	this->nSources = 0;
	generateMoreSources();
}

void AudioManager::destroy() {
	alDeleteSources(this->nSources, this->sources);
	alcDestroyContext(this->context);
	alcCloseDevice(this->device);
}

ALuint AudioManager::RequestALSource() {
	if (this->availableSources.size() > 0) {
		ALuint source = this->availableSources.back();
		this->availableSources.pop_back();
		if (this->availableSources.size() == 0) {
			generateMoreSources();
		}
		return source;
	}
	return 0;
}

void AudioManager::ReleaseALSource(ALuint source) {
	if (source != 0) {
		auto iter = std::find(this->availableSources.begin(), this->availableSources.end(), source);
		if (iter == this->availableSources.end()) {
			this->availableSources.push_back(source);
		}
	}
}

void AudioManager::PauseAllAudio() {
	for (int i = 0; i < this->nSources; ++i) {
		ALint sourceState;
		alGetSourcei(this->sources[i], AL_SOURCE_STATE, &sourceState);
		if (sourceState == AL_PLAYING) {
			alSourcePause(this->sources[i]);
		}
	}
}

void AudioManager::ResumeAllAudio() {
	for (int i = 0; i < this->nSources; ++i) {
		ALint sourceState;
		alGetSourcei(this->sources[i], AL_SOURCE_STATE, &sourceState);
		if (sourceState == AL_PAUSED) {
			alSourcePlay(this->sources[i]);
		}
	}
}

void AudioManager::StopAllAudio() {
	for (int i = 0; i < this->nSources; ++i) {
		alSourceStop(this->sources[i]);
	}
}

void AudioManager::generateMoreSources() {
	ASSERT(this->nSources < MAXIMUM_AUDIO_SOURCES, "Able to generate additional audio sources");
	if (this->nSources < MAXIMUM_AUDIO_SOURCES) {
		alGenSources(SOURCE_CHUNK_SIZE, this->sources + this->nSources);
		ALenum err = alGetError();
		ASSERT(err == AL_NO_ERROR, "Successfully generated additional audio sources");
		if (err == AL_NO_ERROR) {
			for (int i = this->nSources; i < this->nSources + SOURCE_CHUNK_SIZE; ++i) {
				this->availableSources.push_back(this->sources[i]);
			}
			this->nSources += SOURCE_CHUNK_SIZE;
		}
	}
}
