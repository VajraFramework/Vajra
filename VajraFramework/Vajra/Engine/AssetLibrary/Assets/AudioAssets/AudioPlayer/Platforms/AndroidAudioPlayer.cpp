#ifdef PLATFORM_ANDROID

#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/AudioAssets/AudioAsset.h"
#include "Vajra/Engine/AssetLibrary/Assets/AudioAssets/AudioPlayer/AudioPlayer.h"
#include "Vajra/Engine/Core/Engine.h"

AudioPlayer::AudioPlayer() {
	init();
}

AudioPlayer::~AudioPlayer() {
	destroy();
}

void AudioPlayer::init() {
	// this->pimpl = new AudioPlayerHelper_Impl;
	this->volume = 1.0f;
	this->playbackSpeed = 1.0f;
}

void AudioPlayer::destroy() {
#if 0
	if (this->pimpl != nullptr) {
		if (this->pimpl->player_internal != nil) {
			[this->pimpl->player_internal release];
			this->pimpl->player_internal = nil;
		}
		delete this->pimpl;
		this->pimpl = nullptr;
	}
#endif
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
#if 0
	NSData *audioData = [NSData dataWithBytes:audioAsset->GetAudioData() length:audioAsset->GetAudioLength()];
	NSError *errOut;
	this->pimpl->player_internal = [this->pimpl->player_internal initWithData: audioData error:&errOut];
	if (this->pimpl->player_internal != nil) {
		this->pimpl->player_internal.volume = this->volume;
		this->pimpl->player_internal.rate = this->playbackSpeed;
		this->pimpl->clipLoaded = true;
	}
#endif
}

void AudioPlayer::SetVolume(float volume) {
	this->volume = volume;
#if 0
	if (this->pimpl->clipLoaded) {
		this->pimpl->player_internal.volume = this->volume;
	}
#endif
}

void AudioPlayer::SetPlaybackSpeed(float speed) {
	this->playbackSpeed = speed;
#if 0
	if (this->pimpl->clipLoaded) {
		this->pimpl->player_internal.rate = this->playbackSpeed;
	}
#endif
}

void AudioPlayer::Play() {
#if 0
	if (this->pimpl->clipLoaded) {
		[this->pimpl->player_internal prepareToPlay];
		[this->pimpl->player_internal play];
	}
#endif
}

void AudioPlayer::Play(float delay) {
#if 0
	if (this->pimpl->clipLoaded) {
		[this->pimpl->player_internal prepareToPlay];
		[this->pimpl->player_internal playAtTime:(this->pimpl->player_internal.deviceCurrentTime + delay)];
	}
#endif
}

void AudioPlayer::Pause() {
#if 0
	if (this->pimpl->clipLoaded) {
		[this->pimpl->player_internal stop];
	}
#endif
}

void AudioPlayer::Stop() {
#if 0
	if (this->pimpl->clipLoaded) {
		Stop(0.0f);
	}
#endif
}

void AudioPlayer::Stop(float fadeout) {
#if 0
	if (this->pimpl->clipLoaded) {
		[this->pimpl->player_internal stop];
		this->pimpl->player_internal.currentTime = 0.0f;
	}
#endif
}

#endif
