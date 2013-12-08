//
//  AudioManager.cpp
//  Created by Matt Kaufmann on 12/04/13.
//

#include "Vajra/Engine/AudioManager/AudioManager.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

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
}

void AudioManager::destroy() {
	alcDestroyContext(this->context);
	alcCloseDevice(this->device);
}
