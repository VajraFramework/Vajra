//
//	AudioListener.cpp
//	Created by Matt Kaufmann on 04/10/14.
//

#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/AudioManager/AudioManager.h"
#include "Vajra/Engine/Components/ComponentTypes/ComponentTypeIds.h"
#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioListener.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"

unsigned int AudioListener::componentTypeId = COMPONENT_TYPE_ID_AUDIO_LISTENER;
ObjectIdType AudioListener::activeListener  = OBJECT_ID_INVALID;

// Constructors
AudioListener::AudioListener() : Component() {
	this->init();
}

AudioListener::AudioListener(Object* object_) : Component(object_) {
	this->init();
}

// Destructor
AudioListener::~AudioListener() {
	this->destroy();
}

bool AudioListener::Is3DSoundEnabled() {
	return this->is3D;
}

void AudioListener::Enable3DSound() {
	this->is3D = true;
	if (AudioListener::activeListener == this->GetObject()->GetId()) {
		ENGINE->GetAudioManager()->Enable3DSound();
	}
}

void AudioListener::Disable3DSound() {
	this->is3D = false;
	if (AudioListener::activeListener == this->GetObject()->GetId()) {
		ENGINE->GetAudioManager()->Disable3DSound();
	}
}

void AudioListener::SetVelocity(glm::vec3 vel) {
	this->velocity = vel;
	if (AudioListener::activeListener == this->GetObject()->GetId()) {
		ENGINE->GetAudioManager()->SetListenerVelocity(vel);
	}
}

void AudioListener::SetVelocity(float x, float y, float z) {
	this->velocity = glm::vec3(x, y, z);
	if (AudioListener::activeListener == this->GetObject()->GetId()) {
		ENGINE->GetAudioManager()->SetListenerVelocity(x, y, z);
	}
}

void AudioListener::SetVolume(float vol) {
	this->volume = vol;
	if (AudioListener::activeListener == this->GetObject()->GetId()) {
		ENGINE->GetAudioManager()->SetListenerVolume(vol);
	}
}

void AudioListener::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT:
			onTransformChanged();
			break;
	}
}

void AudioListener::SetAsActiveListener() {
	if (AudioListener::activeListener != this->GetObject()->GetId()) {
		if (this->is3D) {
			ENGINE->GetAudioManager()->Enable3DSound();
		}
		else {
			ENGINE->GetAudioManager()->Disable3DSound();
		}

		Transform* trans = this->GetObject()->GetComponent<Transform>();
		ENGINE->GetAudioManager()->SetListenerPosition(trans->GetPositionWorld());
		ENGINE->GetAudioManager()->SetListenerOrientation(trans->GetOrientationWorld());

		ENGINE->GetAudioManager()->SetListenerVelocity(this->velocity);
		ENGINE->GetAudioManager()->SetListenerVolume(this->volume);

		AudioListener::activeListener = this->GetObject()->GetId();
	}
}

void AudioListener::init() {
	this->is3D = false;
	this->velocity = ZERO_VEC3;
	this->volume = 1.0f;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT, this->GetTypeId(), true);
}

void AudioListener::destroy() {

}

void AudioListener::onTransformChanged() {
	if (AudioListener::activeListener == this->GetObject()->GetId()) {
		Transform* trans = this->GetObject()->GetComponent<Transform>();
		ENGINE->GetAudioManager()->SetListenerPosition(trans->GetPositionWorld());
		ENGINE->GetAudioManager()->SetListenerOrientation(trans->GetOrientationWorld());
	}
}
