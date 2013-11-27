//
//  iOSAudioPlayer.m
//  Created by Matt Kaufmann on 11/25/13.
//  Copyright (c) 2013 Matt Kaufmann. All rights reserved.
//

#ifdef PLATFORM_IOS

#import <AVFoundation/AVFoundation.h>

#import "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#import "Vajra/Engine/AssetLibrary/Assets/AudioAssets/AudioAsset.h"
#import "Vajra/Engine/AssetLibrary/Assets/AudioAssets/AudioPlayer/AudioPlayer.h"
#import "Vajra/Engine/Core/Engine.h"

struct AudioPlayer::AudioPlayerHelper_Impl {
	AVAudioPlayer *avap;
	bool clipLoaded;
	
	AudioPlayerHelper_Impl() : avap([[AVAudioPlayer alloc] init]), clipLoaded(false) { }
};

AudioPlayer::AudioPlayer() {
	init();
}

AudioPlayer::~AudioPlayer() {
	destroy();
}

void AudioPlayer::init() {
	this->pimpl = new AudioPlayerHelper_Impl;
	//this->pimpl->avap = [[AVAudioPlayer alloc] init];
	this->volume = 1.0f;
	this->playbackSpeed = 1.0f;
}

void AudioPlayer::destroy() {
	if (this->pimpl != nullptr) {
		if (this->pimpl->avap != nil) {
			[this->pimpl->avap release];
			this->pimpl->avap = nil;
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
		return this->pimpl->avap.duration;
	}
	return 0.0f;
}

// Mutators
void AudioPlayer::SetAudioClip(std::string assetName) {
	std::shared_ptr<AudioAsset> audioAsset = ENGINE->GetAssetLibrary()->GetAsset<AudioAsset>(assetName);
	NSData *audioData = [NSData dataWithBytes:audioAsset->GetAudioData() length:audioAsset->GetAudioLength()];
	NSError *errOut;
	this->pimpl->avap = [this->pimpl->avap initWithData: audioData error:&errOut];
	if (this->pimpl->avap != nil) {
		this->pimpl->avap.volume = this->volume;
		this->pimpl->avap.rate = this->playbackSpeed;
		this->pimpl->clipLoaded = true;
	}
}

void AudioPlayer::SetVolume(float volume) {
	this->volume = volume;
	if (this->pimpl->clipLoaded) {
		this->pimpl->avap.volume = volume;
	}
}

void AudioPlayer::SetPlaybackSpeed(float speed) {
	this->playbackSpeed = speed;
	if (this->pimpl->clipLoaded) {
		this->pimpl->avap.rate = speed;
	}
}

void AudioPlayer::Play() {
	if (this->pimpl->clipLoaded) {
		[this->pimpl->avap prepareToPlay];
		[this->pimpl->avap play];
	}
}

void AudioPlayer::Play(float delay) {
	if (this->pimpl->clipLoaded) {
		[this->pimpl->avap prepareToPlay];
		[this->pimpl->avap playAtTime:(this->pimpl->avap.deviceCurrentTime + delay)];
	}
}

void AudioPlayer::Pause() {
	if (this->pimpl->clipLoaded) {
		[this->pimpl->avap stop];
	}
}

void AudioPlayer::Stop() {
	if (this->pimpl->clipLoaded) {
		Stop(0.0f);
	}
}

void AudioPlayer::Stop(float fadeout) {
	if (this->pimpl->clipLoaded) {
		[this->pimpl->avap stop];
		this->pimpl->avap.currentTime = 0.0f;
	}
}

#endif