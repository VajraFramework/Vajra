//
//  TriggerParticles.cpp
//  Created by Matt Kaufmann on 04/17/14.
//

#include "ExampleGame/Components/Triggers/TriggerParticles.h"
#include "Vajra/Common/Objects/Object.h"

TriggerParticles::TriggerParticles() : Triggerable() {
	this->init();
}

TriggerParticles::TriggerParticles(Object* object_) : Triggerable(object_) {
	this->init();
}

TriggerParticles::~TriggerParticles() {
	this->destroy();
}

void TriggerParticles::init() {
	this->looped         = false;
	this->particleSystem = nullptr;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_SCENE_START, this->GetTypeId(), false);
}

void TriggerParticles::destroy() {

}

void TriggerParticles::SetTriggerType(std::string typeStr) {
	Triggerable::SetTriggerType(typeStr);
}

void TriggerParticles::SetLooped(bool loop) {
	this->looped = loop;
}

void TriggerParticles::HandleMessage(MessageChunk messageChunk) {
	Triggerable::HandleMessage(messageChunk);
}

void TriggerParticles::SubscribeToMySwitch() {
	Triggerable::SubscribeToMySwitch();
}

void TriggerParticles::SubscribeToParentSwitch() {
	Triggerable::SubscribeToParentSwitch();
}

void TriggerParticles::start() {
	this->particleSystem = this->GetObject()->GetComponent<ParticleSystem>();
}

void TriggerParticles::onSwitchToggled(bool /*switchState*/) {
	if (this->particleSystem != nullptr) {
		if (this->looped) {
			if (this->particleSystem->GetIsPlaying()) {
				this->particleSystem->Pause();
			}
			else {
				this->particleSystem->Play();
			}
		}
		else {
			this->particleSystem->Play();
		}
	}
}
