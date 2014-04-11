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
		Transform* trans = this->GetObject()->GetComponent<Transform>();
		ENGINE->GetAudioManager()->SetListenerPosition(trans->GetPositionWorld());
		ENGINE->GetAudioManager()->SetListenerOrientation(trans->GetOrientationWorld());

		AudioListener::activeListener = this->GetObject()->GetId();
	}
}

void AudioListener::init() {
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
