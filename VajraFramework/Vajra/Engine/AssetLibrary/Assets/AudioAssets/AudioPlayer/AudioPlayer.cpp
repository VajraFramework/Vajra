#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AudioAssets/AudioAsset.h"
#include "Vajra/Engine/AssetLibrary/Assets/AudioAssets/AudioPlayer/AudioPlayer.h"
#include "Vajra/Engine/Core/Engine.h"

#ifdef PLATFORM_IOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include "Libraries/openal/headers/al.h"
#include "Libraries/openal/headers/alc.h"
#endif

ALCdevice* AudioPlayer::device = nullptr;
ALCcontext* AudioPlayer::context = nullptr;
bool AudioPlayer::audioReady = false;

void AudioPlayer::InitAudio() {
	// Open an audio device
	device = alcOpenDevice(nullptr);
	if (device == nullptr) {
		return;
	}

	// Create and initialize the audio context
	ALCcontext* context;
	context = alcCreateContext(device, nullptr);
	if (!alcMakeContextCurrent(context)) {
		return;
	}

	// Audio is now ready to use
	audioReady = true;
}

void AudioPlayer::CleanupAudio() {
	alcCloseDevice(device);
	alcDestroyContext(context);
	audioReady = false;
}

bool AudioPlayer::AudioIsReady() { return audioReady; }

AudioPlayer::AudioPlayer() {
	init();
}

AudioPlayer::~AudioPlayer() {
	destroy();
}

void AudioPlayer::init() {
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

float AudioPlayer::GetAudioClipDuration() {
#if 0
	if (this->pimpl->clipLoaded) {
		return this->pimpl->player_internal.duration;
	}
#endif
	return 0.0f;
}

// Mutators
void AudioPlayer::SetAudioClip(std::string assetName) {
	std::shared_ptr<AudioAsset> audioAsset = ENGINE->GetAssetLibrary()->GetAsset<AudioAsset>(assetName);
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
