//
//	AudioSource.h
//	Created by Matt Kaufmann on 11/24/13.
//
//	An AudioSource component can be attached to a game object in order
//	to allow it to play sounds.
//

#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <string>

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/AudioManager/AudioPlayer.h"

class AudioSource : public Component {
public:
	// Constructors
	AudioSource();
	AudioSource(Object* object_);
	//AudioSource(const char* audioClip);
	
	// Destructor
	~AudioSource();
	
	static inline unsigned int GetTypeId() { return componentTypeId; }
	
	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);
	
	// Accessors
	float GetVolume();
	float GetPlaybackSpeed();
	AudioAsset* GetAudioClip();
	
	// Mutators
	void SetAudioClip(std::string assetName);
	void SetVolume(float volume);
	void SetPlaybackSpeed(float speed);
	
	// Other methods
	void Play();
	void Pause();
	void Stop();
	void Stop(float fadeout);
	
private:
	void init();
	void destroy();
	
	AudioPlayer* player;
	
	bool positionalAudio;
	float volume;
	float playbackSpeed;
	
	static unsigned int componentTypeId;
};

#endif // AUDIOSOURCE_H
