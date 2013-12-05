//
//  AudioAsset.h
//  Created by Matt Kaufmann on 11/25/13.
//

#ifndef AUDIO_ASSET_H
#define AUDIO_ASSET_H

#include "Vajra/Engine/AssetLibrary/Asset.h"

#ifdef PLATFORM_IOS
#include <OpenAL/al.h>
#else
#include "Libraries/openal/headers/al.h"
#endif

#include <string>

class AudioAsset : public Asset {
public:
	AudioAsset();
	AudioAsset(std::string urlOfAudioClip);
	virtual ~AudioAsset();
	
	std::string GetFilePathToAudio();
	inline ALuint GetALAudioHandle() { return this->audioALHandle; }
	inline float GetAudioDuration()  { return this->audioDuration; }
	
	// @Override
	virtual AssetType GetAssetType();
	// @Override
	virtual void LoadAsset();
	
private:
	void init();
	void destroy();
	
	static AssetType assetType;
	
	ALubyte* audioBytes;
	ALuint audioALHandle;
	ALenum audioFormat;
	ALsizei audioByteLength;  // Size of the audio data in bytes
	ALsizei audioSampleRate;
	float audioDuration;      // Length of the audio data in seconds
};

#endif // AUDIO_ASSET_H
