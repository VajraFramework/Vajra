#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AudioAssets/AudioAsset.h"
#include "Vajra/Engine/AudioManager/AudioPlayer.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Logging/Logger.h"

#ifdef PLATFORM_IOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include "Libraries/openal/headers/al.h"
#include "Libraries/openal/headers/alc.h"
#endif

AudioPlayer::AudioPlayer() {
	init();
}

AudioPlayer::~AudioPlayer() {
	destroy();
}

void AudioPlayer::init() {
	this->asset = nullptr;
	this->pitch = 1.0f;
	this->gain = 1.0f;
	this->position[0] = 0.0f;
	this->position[1] = 0.0f;
	this->position[2] = 0.0f;
	this->velocity[0] = 0.0f;
	this->velocity[1] = 0.0f;
	this->velocity[2] = 0.0f;
	this->looping = AL_FALSE;

	alGenSources(1, &(this->source));
	alSourcef(this->source, AL_PITCH, this->pitch);
	alSourcef(this->source, AL_GAIN, this->gain);
	alSource3f(this->source, AL_POSITION, this->position[0], this->position[1], this->position[2]);
	alSource3f(this->source, AL_VELOCITY, this->velocity[0], this->velocity[1], this->velocity[2]);
	alSourcei(this->source, AL_LOOPING, this->looping);

	this->volume = 1.0f;
	this->playbackSpeed = 1.0f;
}

void AudioPlayer::destroy() {
	alDeleteSources(1, &(this->source));
}

// Accessors
float AudioPlayer::GetVolume()             { return this->volume; }
float AudioPlayer::GetPlaybackSpeed()      { return this->playbackSpeed; }
AudioAsset* AudioPlayer::GetAudioClip()    { return this->asset; }

// Mutators
void AudioPlayer::SetAudioClip(std::string assetName) {
	std::shared_ptr<AudioAsset> audioAsset = ENGINE->GetAssetLibrary()->GetAsset<AudioAsset>(assetName);
	this->asset = &(*audioAsset);
	alSourcei(this->source, AL_BUFFER, audioAsset->GetALAudioHandle());
}

void AudioPlayer::SetVolume(float volume) {
	this->volume = volume;
	alSourcef(this->source, AL_GAIN, volume);
}

void AudioPlayer::SetPlaybackSpeed(float speed) {
	this->playbackSpeed = speed;
	// TODO [Implement] Determine if OpenAL can change playback speed without affecting pitch
}

void AudioPlayer::Play() {
	alSourcePlay(this->source);
}

void AudioPlayer::Play(float delay) {
	alSourcePlay(this->source);
	// TODO [Implement] Determine if OpenAL supports delayed playback
}

void AudioPlayer::Pause() {
	alSourcePause(this->source);
}

void AudioPlayer::Stop() {
	Stop(0.0f);
}

void AudioPlayer::Stop(float fadeout) {
	alSourceStop(this->source);
	// TODO [Implement] Determine if OpenAL supports fadeout
}
