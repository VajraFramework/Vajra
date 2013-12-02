//
//  AudioAsset.h
//  Created by Matt Kaufmann on 11/25/13.
//

#ifndef AUDIO_ASSET_H
#define AUDIO_ASSET_H

#include <string>

#include "Vajra/Engine/AssetLibrary/Asset.h"

class AudioAsset : public Asset {
public:
	AudioAsset();
	AudioAsset(std::string urlOfAudioClip);
	virtual ~AudioAsset();
	
	std::string GetFilePathToAudio();
	const char* GetAudioData();
	long GetAudioLength();
	
	// @Override
	virtual AssetType GetAssetType();
	// @Override
	virtual void LoadAsset();
	
private:
	void init();
	void destroy();
	
	static AssetType assetType;
	
	char* audioBytes;
	long audioLength;	// Size of audio data in bytes
};

#endif // AUDIO_ASSET_H