//
//  AudioAsset.cpp
//  Created by Matt Kaufmann on 11/25/13.
//

#include "Vajra/Engine/AssetLibrary/Assets/AudioAssets/AudioAsset.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Loaders/AudioLoader/AudioLoader.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

#ifdef PLATFORM_IOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include "Libraries/openal/headers/al.h"
#include "Libraries/openal/headers/alc.h"
#endif

AssetType AudioAsset::assetType = ASSET_TYPE_SOUND_DATA;

AudioAsset::AudioAsset() : Asset() {
	this->init();
}

AudioAsset::AudioAsset(std::string urlOfAudioClip) : Asset(urlOfAudioClip) {
	this->init();
}

AudioAsset::~AudioAsset() {
	this->destroy();
}

void AudioAsset::init() {
	this->audioBytes = nullptr;
	this->audioALHandle = 0;
	this->audioFormat = AL_NONE;
	this->audioByteLength = 0;
	this->audioSampleRate = 0;
	this->audioDuration = 0.0f;
}

void AudioAsset::destroy() {
	if (this->audioBytes != nullptr) {
		delete this->audioBytes;
		this->audioBytes = nullptr;
	}

	if (this->audioALHandle != 0) {
		alDeleteBuffers(1, &(this->audioALHandle));
		this->audioALHandle = 0;
	}
}

AssetType AudioAsset::GetAssetType() {
	return AudioAsset::assetType;
}

void AudioAsset::LoadAsset() {
	ASSERT(this->GetUrl() != "", "Url set when trying to load audio asset");
	
	FRAMEWORK->GetLogger()->dbglog("\nLoading audio asset from url: %s", this->GetFilePathToAudio().c_str());
	
	// Load audio file
	this->audioALHandle = loadALAudioFromWAV(this->GetFilePathToAudio().c_str(), &(this->audioFormat), &(this->audioBytes), &(this->audioByteLength), &(this->audioSampleRate));
	this->audioDuration = float(this->audioByteLength) / this->audioSampleRate;
	ASSERT((this->audioByteLength >= 0) && (this->audioBytes != nullptr), "Successfully loaded audio from url %s", this->GetFilePathToAudio().c_str());
}

std::string AudioAsset::GetFilePathToAudio() {
	return this->GetUrl();
}
