//
//	AudioPlayer.h
//	Created by Matt Kaufmann on 11/25/13.
//
//	The AudioPlayer class wraps all playback for the engine.
//	One AudioPlayer is needed for each instance of a given
//	audio clip to be played at one time.

#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <string>

class AudioPlayer {
public:
	AudioPlayer();
	~AudioPlayer();
	
	// Accessors
	float GetVolume();
	float GetPlaybackSpeed();
	float GetAudioClipDuration();
	
	// Mutators
	void SetAudioClip(std::string assetName);
	void SetVolume(float volume);
	void SetPlaybackSpeed(float speed);
	
	// Other methods
	void Play();
	void Play(float delay);
	void Pause();
	void Stop();
	void Stop(float fadeout);
	
private:
	void init();
	void destroy();
	
	struct AudioPlayerHelper_Impl;
	AudioPlayerHelper_Impl* pimpl;

	float volume;
	float playbackSpeed;
};

#endif // AUDIOPLAYER_H