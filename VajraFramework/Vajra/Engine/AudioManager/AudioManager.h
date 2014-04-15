//
//  AudioManager.h
//  Created by Matt Kaufmann on 12/04/13.
//

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "Libraries/glm/glm.hpp"
#include "Vajra/Common/Objects/Object.h"

#include <list>

class AudioManagerInternal;
class AudioPlayer;

class AudioManager {
public:
	~AudioManager();

	bool Is3DSoundEnabled();

	void Enable3DSound();
	void Disable3DSound();
	void SetListenerPosition(glm::vec3 pos);
	void SetListenerPosition(float x, float y, float z);
	void SetListenerOrientation(glm::quat orient);
	void SetListenerOrientation(glm::vec3 forward, glm::vec3 up);
	void SetListenerVelocity(glm::vec3 pos);
	void SetListenerVelocity(float x, float y, float z);
	void SetListenerVolume(float volume);

	AudioPlayer* RequestAudioPlayer(ObjectIdType objId);
	void ReturnAudioPlayer(ObjectIdType objId, AudioPlayer* player);

	void Pause3dAudio();
	void PauseUiAudio();
	void PauseAllAudio();
	void Resume3dAudio();
	void ResumeUiAudio();
	void ResumeAllAudio();
	void Stop3dAudio();
	void StopUiAudio();
	void StopAllAudio();

private:
	AudioManager();

	void init();
	void destroy();

	AudioManagerInternal* internalMgr;

	std::list<ObjectIdType> sources3D;
	std::list<ObjectIdType> sourcesUI;

	friend class Engine;
};

#endif // AUDIOMANAGER_H
