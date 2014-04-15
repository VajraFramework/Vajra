//
//  AudioManagerInternal.cpp
//  Created by Matt Kaufmann on 04/15/14.
//

#include "Vajra/Engine/AudioManager/AudioManagerInternal.h"
#include "Vajra/Engine/AudioManager/AudioPlayer.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/MathUtilities.h"
#include "Vajra/Utilities/Utilities.h"

#include <algorithm>

AudioManagerInternal::AudioManagerInternal() {
	this->init();
}

AudioManagerInternal::~AudioManagerInternal() {
	this->destroy();
}

void AudioManagerInternal::init() {
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

	this->nSources = 0;
	generateMoreSources();
}

void AudioManagerInternal::destroy() {
	while (this->playersInUse.size() > 0) {
		delete this->playersInUse.front();
		this->playersInUse.pop_front();
	}
	while (this->availablePlayers.size() > 0) {
		delete this->availablePlayers.front();
		this->availablePlayers.pop_front();
	}

	alDeleteSources(this->nSources, this->sources);
	alcDestroyContext(this->context);
	alcCloseDevice(this->device);
}

bool AudioManagerInternal::Is3DSoundEnabled() {
	ALint distModel;
	alGetIntegerv(AL_DISTANCE_MODEL, &distModel);
	return (distModel != AL_NONE);
}

void AudioManagerInternal::Enable3DSound() {
	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
}

void AudioManagerInternal::Disable3DSound() {
	alDistanceModel(AL_NONE);
}

void AudioManagerInternal::SetListenerPosition(glm::vec3 pos) {
	SetListenerPosition(pos.x, pos.y, pos.z);
}

void AudioManagerInternal::SetListenerPosition(float x, float y, float z) {
	alListener3f(AL_POSITION, x, y, z);
}

void AudioManagerInternal::SetListenerOrientation(glm::quat orient) {
	SetListenerOrientation(QuaternionForwardVector(orient), QuaternionUpVector(orient));
}

void AudioManagerInternal::SetListenerOrientation(glm::vec3 forward, glm::vec3 up) {
	ALfloat listenerOri[] = {forward.x, forward.y, forward.z, up.x, up.y, up.z};
	alListenerfv(AL_ORIENTATION, listenerOri);
}

void AudioManagerInternal::SetListenerVelocity(glm::vec3 pos) {
	SetListenerVelocity(pos.x, pos.y, pos.z);
}

void AudioManagerInternal::SetListenerVelocity(float x, float y, float z) {
	alListener3f(AL_VELOCITY, x, y, z);
}

void AudioManagerInternal::SetListenerVolume(float volume) {
	alListenerf(AL_GAIN, volume);
}

AudioPlayer* AudioManagerInternal::RequestAudioPlayer() {
	if (this->availablePlayers.size() > 0) {
		AudioPlayer* player = this->availablePlayers.back();
		this->availablePlayers.pop_back();
		this->playersInUse.push_back(player);
		if (this->availablePlayers.size() == 0) {
			generateMoreSources();
		}
		return player;
	}
	return nullptr;
/*
	if (this->availableSources.size() > 0) {
		ALuint source = this->availableSources.back();
		this->availableSources.pop_back();
		if (this->availableSources.size() == 0) {
			generateMoreSources();
		}
		return source;
	}
	return 0;
*/
}

void AudioManagerInternal::ReturnAudioPlayer(AudioPlayer* player) {
	auto iter = std::find(this->playersInUse.begin(), this->playersInUse.end(), player);
	if (iter != this->playersInUse.end()) {
		this->playersInUse.erase(iter);
		this->availablePlayers.push_back(player);
	}
}
/*
ALuint AudioManagerInternal::RequestALSource() {
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

void AudioManagerInternal::ReleaseALSource(ALuint source) {
	if (source != 0) {
		auto iter = std::find(this->availableSources.begin(), this->availableSources.end(), source);
		if (iter == this->availableSources.end()) {
			this->availableSources.push_back(source);
		}
	}
}
*/
void AudioManagerInternal::PauseAllAudio() {
	for (int i = 0; i < this->nSources; ++i) {
		ALint sourceState;
		alGetSourcei(this->sources[i], AL_SOURCE_STATE, &sourceState);
		if (sourceState == AL_PLAYING) {
			alSourcePause(this->sources[i]);
		}
	}
}

void AudioManagerInternal::ResumeAllAudio() {
	for (int i = 0; i < this->nSources; ++i) {
		ALint sourceState;
		alGetSourcei(this->sources[i], AL_SOURCE_STATE, &sourceState);
		if (sourceState == AL_PAUSED) {
			alSourcePlay(this->sources[i]);
		}
	}
}

void AudioManagerInternal::StopAllAudio() {
	for (int i = 0; i < this->nSources; ++i) {
		alSourceStop(this->sources[i]);
	}
}

void AudioManagerInternal::generateMoreSources() {
	ASSERT(this->nSources < MAXIMUM_AUDIO_SOURCES, "Able to generate additional audio sources");
	if (this->nSources < MAXIMUM_AUDIO_SOURCES) {
		alGenSources(SOURCE_CHUNK_SIZE, this->sources + this->nSources);
		ALenum err = alGetError();
		ASSERT(err == AL_NO_ERROR, "Able to generate %d additional audio sources (%d currently)", SOURCE_CHUNK_SIZE, this->nSources);
		if (err == AL_NO_ERROR) {
			for (int i = this->nSources; i < this->nSources + SOURCE_CHUNK_SIZE; ++i) {
				AudioPlayer* player = new AudioPlayer();
				player->SetALSource(this->sources[i]);
				//this->availableSources.push_back(this->sources[i]);
				this->availablePlayers.push_back(player);
			}
			this->nSources += SOURCE_CHUNK_SIZE;
		}
	}
}
