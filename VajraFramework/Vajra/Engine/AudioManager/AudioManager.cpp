//
//  AudioManager.cpp
//  Created by Matt Kaufmann on 12/04/13.
//

#include "Vajra/Engine/AudioManager/AudioManager.h"
#include "Vajra/Engine/AudioManager/AudioManagerInternal.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/MathUtilities.h"
#include "Vajra/Utilities/Utilities.h"

#include <algorithm>

AudioManager::AudioManager() {
	this->init();
}

AudioManager::~AudioManager() {
	this->destroy();
}

void AudioManager::init() {
	this->internalMgr = new AudioManagerInternal();

	// Set the default listener for OpenAL.
	SetListenerPosition(0.0f, 0.0f, 0.0f);
	SetListenerVelocity(0.0f, 0.0f, 0.0f);
	SetListenerOrientation(ZAXIS, YAXIS);
	SetListenerVolume(1.0f);
}

void AudioManager::destroy() {
	if (this->internalMgr != nullptr) {
		delete this->internalMgr;
		this->internalMgr = nullptr;
	}
}

bool AudioManager::Is3DSoundEnabled() {
	return this->internalMgr->Is3DSoundEnabled();
}

void AudioManager::Enable3DSound() {
	this->internalMgr->Enable3DSound();
}

void AudioManager::Disable3DSound() {
	this->internalMgr->Disable3DSound();
}

void AudioManager::SetListenerPosition(glm::vec3 pos) {
	this->internalMgr->SetListenerPosition(pos);
}

void AudioManager::SetListenerPosition(float x, float y, float z) {
	this->internalMgr->SetListenerPosition(x, y, z);
}

void AudioManager::SetListenerOrientation(glm::quat orient) {
	this->internalMgr->SetListenerOrientation(orient);
}

void AudioManager::SetListenerOrientation(glm::vec3 forward, glm::vec3 up) {
	this->internalMgr->SetListenerOrientation(forward, up);
}

void AudioManager::SetListenerVelocity(glm::vec3 pos) {
	this->internalMgr->SetListenerVelocity(pos);
}

void AudioManager::SetListenerVelocity(float x, float y, float z) {
	this->internalMgr->SetListenerVelocity(x, y, z);
}

void AudioManager::SetListenerVolume(float volume) {
	this->internalMgr->SetListenerVolume(volume);
}

AudioPlayer* AudioManager::RequestAudioPlayer() {
	return this->internalMgr->RequestAudioPlayer();
}

void AudioManager::ReturnAudioPlayer(AudioPlayer* player) {
	this->internalMgr->ReturnAudioPlayer(player);
}
/*
void AudioManager::ReleaseALSource(ALuint source) {
	if (source != 0) {
		auto iter = std::find(this->availableSources.begin(), this->availableSources.end(), source);
		if (iter == this->availableSources.end()) {
			this->availableSources.push_back(source);
		}
	}
}
*/
void AudioManager::PauseAllAudio() {
	this->internalMgr->PauseAllAudio();
}

void AudioManager::ResumeAllAudio() {
	this->internalMgr->ResumeAllAudio();
}

void AudioManager::StopAllAudio() {
	this->internalMgr->StopAllAudio();
}
