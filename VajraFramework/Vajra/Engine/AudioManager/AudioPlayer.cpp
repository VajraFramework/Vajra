#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AudioManager/AudioManager.h"
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
	this->source = 0;
}

void AudioPlayer::destroy() {
	Stop();
}

// Mutators
void AudioPlayer::SetALSource(ALuint s) {
	this->source = s;
	SetPosition(0.0f, 0.0f, 0.0f);
	SetVelocity(0.0f, 0.0f, 0.0f);
	SetVolume(1.0f);
	SetLooping(AL_FALSE);
	alSourcef(this->source, AL_PITCH, 1.0f);
}

void AudioPlayer::SetAudioClip(std::string assetName) {
	if ((this->asset == nullptr) || (this->asset->GetUrl() != assetName)) {
		Stop();
		if (assetName != "") {
			this->asset = ENGINE->GetAssetLibrary()->GetAsset<AudioAsset>(assetName);
			alSourcei(this->source, AL_BUFFER, this->asset->GetALAudioHandle());
		}
		else {
			this->asset = nullptr;
			alSourcei(this->source, AL_BUFFER, 0);
		}
	}
}

void AudioPlayer::SetAudioClip(std::shared_ptr<AudioAsset> assetPtr) {
	if (this->asset != assetPtr) {
		Stop();
		this->asset = assetPtr;
		if (this->asset != nullptr) {
			alSourcei(this->source, AL_BUFFER, this->asset->GetALAudioHandle());
		}
		else {
			alSourcei(this->source, AL_BUFFER, 0);
		}
	}
}

void AudioPlayer::SetPosition(glm::vec3 position) {
	SetPosition(position.x, position.y, position.z);
}

void AudioPlayer::SetPosition(float x, float y, float z) {
	alSource3f(this->source, AL_POSITION, x, y, z);
}

void AudioPlayer::SetPositionIsRelative(bool isRelative) {
	if (isRelative) {
		alSourcei(this->source, AL_SOURCE_RELATIVE, AL_TRUE);
	}
	else {
		alSourcei(this->source, AL_SOURCE_RELATIVE, AL_FALSE);
	}
}

void AudioPlayer::SetVelocity(glm::vec3 velocity) {
	SetVelocity(velocity.x, velocity.y, velocity.z);
}

void AudioPlayer::SetVelocity(float x, float y, float z) {
	alSource3f(this->source, AL_VELOCITY, x, y, z);
}

void AudioPlayer::SetVolume(float volume) {
	this->volume = volume;
	alSourcef(this->source, AL_GAIN, volume);
}

void AudioPlayer::SetPlaybackSpeed(float speed) {
	this->playbackSpeed = speed;
	// TODO [Implement] Determine if OpenAL can change playback speed without affecting pitch
}

void AudioPlayer::SetLooping(bool loop) {
	if (loop) {
		alSourcei(this->source, AL_LOOPING, AL_TRUE);
	}
	else {
		alSourcei(this->source, AL_LOOPING, AL_FALSE);
	}
}

void AudioPlayer::Play() {
	alSourcePlay(this->source);
}

void AudioPlayer::Play(float /* delay */) {
	alSourcePlay(this->source);
	// TODO [Implement] Determine if OpenAL supports delayed playback
}

void AudioPlayer::Pause() {
	alSourcePause(this->source);
}

void AudioPlayer::Stop() {
	Stop(0.0f);
}

void AudioPlayer::Stop(float /* fadeout */) {
	alSourceStop(this->source);
	// TODO [Implement] Determine if OpenAL supports fadeout
}
