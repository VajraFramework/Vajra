//
//  iOSAudioPlayer.mm
//  Created by Matt Kaufmann on 11/25/13.
//

#ifdef PLATFORM_IOS

#import <AVFoundation/AVFoundation.h>

#import "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#import "Vajra/Engine/AssetLibrary/Assets/AudioAssets/AudioAsset.h"
#import "Vajra/Engine/AssetLibrary/Assets/AudioAssets/AudioPlayer/AudioPlayer.h"
#import "Vajra/Engine/Core/Engine.h"

struct AudioPlayer::AudioPlayerHelper_Impl {
	AVAudioPlayer *player_internal;
	bool clipLoaded;
	
	AudioPlayerHelper_Impl() : player_internal([[AVAudioPlayer alloc] init]), clipLoaded(false) { }
};

AudioPlayer::AudioPlayer() {
	init();
}

AudioPlayer::~AudioPlayer() {
	destroy();
}

void AudioPlayer::init() {
	this->pimpl = new AudioPlayerHelper_Impl;
	this->volume = 1.0f;
	this->playbackSpeed = 1.0f;
}

void AudioPlayer::destroy() {
	if (this->pimpl != nullptr) {
		if (this->pimpl->player_internal != nil) {
			[this->pimpl->player_internal release];
			this->pimpl->player_internal = nil;
		}
		delete this->pimpl;
		this->pimpl = nullptr;
	}
}

// Accessors
float AudioPlayer::GetVolume()             { return this->volume; }
float AudioPlayer::GetPlaybackSpeed()      { return this->playbackSpeed; }

float AudioPlayer::GetAudioClipDuration() {
	if (this->pimpl->clipLoaded) {
		return this->pimpl->player_internal.duration;
	}
	return 0.0f;
}

// Mutators
void AudioPlayer::SetAudioClip(std::string assetName) {
	std::shared_ptr<AudioAsset> audioAsset = ENGINE->GetAssetLibrary()->GetAsset<AudioAsset>(assetName);
	NSData *audioData = [NSData dataWithBytes:audioAsset->GetAudioData() length:audioAsset->GetAudioLength()];
	NSError *errOut;
	this->pimpl->player_internal = [this->pimpl->player_internal initWithData: audioData error:&errOut];
	if (this->pimpl->player_internal != nil) {
		this->pimpl->player_internal.volume = this->volume;
		this->pimpl->player_internal.rate = this->playbackSpeed;
		this->pimpl->clipLoaded = true;
	}
}

void AudioPlayer::SetVolume(float volume) {
	this->volume = volume;
	if (this->pimpl->clipLoaded) {
		this->pimpl->player_internal.volume = this->volume;
	}
}

void AudioPlayer::SetPlaybackSpeed(float speed) {
	this->playbackSpeed = speed;
	if (this->pimpl->clipLoaded) {
		this->pimpl->player_internal.rate = this->playbackSpeed;
	}
}

void AudioPlayer::Play() {
	if (this->pimpl->clipLoaded) {
		[this->pimpl->player_internal prepareToPlay];
		[this->pimpl->player_internal play];
	}
}

void AudioPlayer::Play(float delay) {
	if (this->pimpl->clipLoaded) {
		[this->pimpl->player_internal prepareToPlay];
		[this->pimpl->player_internal playAtTime:(this->pimpl->player_internal.deviceCurrentTime + delay)];
	}
}

void AudioPlayer::Pause() {
	if (this->pimpl->clipLoaded) {
		[this->pimpl->player_internal stop];
	}
}

void AudioPlayer::Stop() {
	if (this->pimpl->clipLoaded) {
		Stop(0.0f);
	}
}

void AudioPlayer::Stop(float fadeout) {
	if (this->pimpl->clipLoaded) {
		[this->pimpl->player_internal stop];
		this->pimpl->player_internal.currentTime = 0.0f;
	}
}

#endif