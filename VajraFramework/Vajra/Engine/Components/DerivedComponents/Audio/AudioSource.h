//
//	AudioSource.h
//	Created by Matt Kaufmann on 11/24/13.
//
//	An AudioSource component can be attached to a game object in order
//	to allow it to play sounds.
//

#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/AssetLibrary/Assets/AudioAssets/AudioAsset.h"
#include "Vajra/Engine/AudioManager/AudioPlayer.h"

#include <map>
#include <memory>
#include <string>

//[[COMPONENT]]//
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
	inline float GetVolume()                           { return this->player->GetVolume();        }
	inline float GetPlaybackSpeed()                    { return this->player->GetPlaybackSpeed(); }
	inline std::shared_ptr<AudioAsset>& GetAudioClip() { return this->player->GetAudioClip();     }
	
	// Mutators
	//[[PROPERTY]]//
	void LoadAudioClip(std::string key, std::string assetName);
	//[[PROPERTY]]//
	void SetAudioClip(std::string key);
	//[[PROPERTY]]//
	void SetVolume(float volume);
	//[[PROPERTY]]//
	void SetPlaybackSpeed(float speed);
	//[[PROPERTY]]//
	void SetLooping(bool loop);

	// Other methods
	void Play();
	void Play(std::string key, bool loop = false);
	void Pause();
	void Stop();
	void Stop(float fadeout);
	
private:
	void init();
	void destroy();
	
	std::map<std::string, std::shared_ptr<AudioAsset>> loadedAssets;

	AudioPlayer* player;
	
	bool positionalAudio;
	float volume;
	float playbackSpeed;
	
	static unsigned int componentTypeId;
};

#endif // AUDIOSOURCE_H
