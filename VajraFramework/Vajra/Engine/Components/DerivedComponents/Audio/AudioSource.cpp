//
//  AudioSource.cpp
//  iOSAudioTest
//
//  Created by IGMAdmin on 11/24/13.
//  Copyright (c) 2013 IGMAdmin. All rights reserved.
//

#include "Vajra/Engine/Components/ComponentTypes/ComponentTypeIds.h"
#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioSource.h"

unsigned int AudioSource::componentTypeId = COMPONENT_TYPE_ID_AUDIO_SOURCE;

// Constructors
AudioSource::AudioSource() : Component() {
	init();
}

AudioSource::AudioSource(Object* object_) : Component(object_) {
	init();
}
/*
AudioSource::AudioSource(const char* audioClip) :
	positionalAudio(false),
	volume(1.0f),
	playbackSpeed(1.0f)
{
	this->player = new AudioPlayer();
	this->player->SetAudioClip(audioClip);
	this->player->SetVolume(this->volume);
	this->player->SetPlaybackSpeed(this->playbackSpeed);
}
*/
// Destructor
AudioSource::~AudioSource() {
	destroy();
}

void AudioSource::init() {
	this->positionalAudio = false;
	this->volume = 1.0f;
	this->playbackSpeed = 1.0f;
	this->player = new AudioPlayer();
	this->player->SetVolume(this->volume);
	this->player->SetPlaybackSpeed(this->playbackSpeed);
}

void AudioSource::destroy() {
	if (this->player != nullptr) {
		delete this->player;
		this->player = nullptr;
	}
}

void AudioSource::HandleMessage(Message *message) {
	// TODO [Implement]
}

// Accessors
float AudioSource::GetVolume()				{ return this->player->GetVolume(); }
float AudioSource::GetPlaybackSpeed()		{ return this->player->GetPlaybackSpeed(); }
float AudioSource::GetAudioClipDuration()	{ return this->player->GetAudioClipDuration(); }

// Mutators
void AudioSource::SetAudioClip(std::string assetName) {
	this->player->SetAudioClip(assetName);
}

void AudioSource::SetVolume(float volume)		{ this->player->SetVolume(volume); }
void AudioSource::SetPlaybackSpeed(float speed)	{ this->player->SetPlaybackSpeed(speed); }

// Other methods
void AudioSource::Play() {
	this->player->Play();
}

void AudioSource::Pause() {
	this->player->Pause();
}

void AudioSource::Stop() {
	this->player->Stop();
}

void AudioSource::Stop(float fadeout) {
	this->player->Stop(fadeout);
}