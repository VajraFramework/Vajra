#include "Vajra/Engine/AudioManager/AudioManager.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

AudioManager::AudioManager() {
	this->init();
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
}

void AudioManager::destroy() {
	alcDestroyContext(this->context);
	alcCloseDevice(this->device);
}
