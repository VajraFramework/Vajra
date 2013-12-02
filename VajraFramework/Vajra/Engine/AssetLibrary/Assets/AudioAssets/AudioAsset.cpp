//
//  AudioAsset.cpp
//  Created by Matt Kaufmann on 11/25/13.
//

#include "Vajra/Engine/AssetLibrary/Assets/AudioAssets/AudioAsset.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/AudioLoader/AudioLoader.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

AssetType AudioAsset::assetType = ASSET_TYPE_SOUND_DATA;

AudioAsset::AudioAsset() : Asset() {
	this->init();
}

AudioAsset::AudioAsset(std::string urlOfAudioClip) : Asset(urlOfAudioClip) {
	this->init();
}

AudioAsset::~AudioAsset() {
}

void AudioAsset::init() {
}

void AudioAsset::destroy() {
}

AssetType AudioAsset::GetAssetType() {
	return AudioAsset::assetType;
}

void AudioAsset::LoadAsset() {
	ASSERT(this->GetUrl() != "", "Url set when trying to load audio asset");
	
	FRAMEWORK->GetLogger()->dbglog("\nLoading audio asset from url: %s", this->GetFilePathToAudio().c_str());
	
	// TODO [Implement] Move loadAudioFile into a Framework class/namespace
	// Load audio file
	this->audioLength = loadAudioFile(this->GetFilePathToAudio().c_str(), &(this->audioBytes));
	ASSERT((this->audioLength >= 0) && (this->audioBytes != nullptr), "Successfully loaded audio from url %s", this->GetFilePathToAudio().c_str());
}

std::string AudioAsset::GetFilePathToAudio() {
	return this->GetUrl();
}

const char* AudioAsset::GetAudioData() {
	return this->audioBytes;
}

long AudioAsset::GetAudioLength() {
	return this->audioLength;
}