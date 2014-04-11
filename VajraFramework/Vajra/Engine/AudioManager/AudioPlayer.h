//
//	AudioPlayer.h
//	Created by Matt Kaufmann on 11/25/13.
//
//	The AudioPlayer class wraps all playback for the engine.
//	One AudioPlayer is needed for each instance of a given
//	audio clip to be played at one time.

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "Libraries/glm/glm.hpp"

#ifdef PLATFORM_IOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include "Libraries/openal/headers/al.h"
#include "Libraries/openal/headers/alc.h"
#endif

#include "Vajra/Engine/AssetLibrary/Assets/AudioAssets/AudioAsset.h"

#include <memory>
#include <string>

class AudioPlayer {
public:
	static void InitAudio();    // Call this before creating any AudioPlayer objects.
	static void CleanupAudio(); // Call this once you're done with all audio in the program.
	static bool AudioIsReady(); // Returns false if audio has not been init'd yet.

	AudioPlayer();
	~AudioPlayer();
	
	// Accessors
	float GetVolume()                                  { return this->volume;        }
	float GetPlaybackSpeed()                           { return this->playbackSpeed; }
	inline std::shared_ptr<AudioAsset>& GetAudioClip() { return this->asset;         }
	
	// Mutators
	void SetAudioClip(std::string assetName);
	void SetAudioClip(std::shared_ptr<AudioAsset> assetPtr);
	void SetPosition(glm::vec3 position);
	void SetPosition(float x, float y, float z);
	void SetPositionIsRelative(bool isRelative);
	void SetVelocity(glm::vec3 velocity);
	void SetVelocity(float x, float y, float z);
	void SetVolume(float volume);
	void SetPlaybackSpeed(float speed);
	void SetLooping(bool loop);
	
	// Other methods
	void Play();
	void Play(float delay);
	void Pause();
	void Stop();
	void Stop(float fadeout);
	
private:
	void init();
	void destroy();

	std::shared_ptr<AudioAsset> asset;
	ALuint source;

	float volume;
	float playbackSpeed;
};

#endif // AUDIOPLAYER_H
