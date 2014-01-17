//
//  AiPerception.cpp
//  Created by Matt Kaufmann on 01/17/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Ai/AiPerception.h"

ComponentIdType AiPerception::componentTypeId = COMPONENT_TYPE_ID_AI_PERCEPTION;

AiPerception::AiPerception() : Component() {
	this->init();
}

AiPerception::AiPerception(Object* object_) : Component(object_) {
	this->init();
}

AiPerception::~AiPerception() {
	this->destroy();
}

void AiPerception::update() {

}

void AiPerception::init() {
	this->knowledge = nullptr;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void AiPerception::destroy() {

}

void AiPerception::gatherInformation() {

}

void AiPerception::gatherInformationAboutObject(ObjectIdType /*objId*/) {

}

float AiPerception::calculateAwarenessOfObject(ObjectIdType objId) {
	// Determine the better awareness by sight or by sound.
	float visual = this->calculateVisualAwareness(objId);
	float audio = this->calculateAudioAwareness(objId);

	if (visual >= audio) {
		return visual;
	}
	return audio;
}

float AiPerception::calculateVisualAwareness(ObjectIdType /*objId*/) {
	return 0.0f;
}

float AiPerception::calculateAudioAwareness(ObjectIdType /*objId*/) {
	return 0.0f;
}
