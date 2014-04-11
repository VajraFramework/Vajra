//
//	AudioListener.h
//	Created by Matt Kaufmann on 04/10/14.
//
//  An AudioListener component can be attached to a game object
//  to make it serve as the player's ears in the game world.
//

#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H

#include "Vajra/Common/Components/Component.h"

//[[COMPONENT]]//
class AudioListener : public Component {
public:
	// Constructors
	AudioListener();
	AudioListener(Object* object_);

	// Destructor
	~AudioListener();

	static inline unsigned int GetTypeId() { return componentTypeId; }

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

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	void SetAsActiveListener();  // Make sure you explicitly set someone as the active listener

private:
	void init();
	void destroy();

	void onTransformChanged();



	static unsigned int componentTypeId;
	static ObjectIdType activeListener;
};


#endif // AUDIOLISTENER_H
