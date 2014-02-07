#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/ParticleSystems/ParticleSystem.h"
#include "Vajra/Engine/Timer/Timer.h"

unsigned int ParticleSystem::componentTypeId = COMPONENT_TYPE_ID_PARTICLE_SYSTEM;

ParticleSystem::ParticleSystem() : Component() {
	this->init();
}

ParticleSystem::ParticleSystem(Object* object_) : Component(object_) {
	this->init();
}

ParticleSystem::~ParticleSystem() {
	this->destroy();
}

void ParticleSystem::start() {
}

void ParticleSystem::update() {
	this->stepSimulation((float)ENGINE->GetTimer()->GetDeltaFrameTime());
}

void ParticleSystem::end() {
}

void ParticleSystem::stepSimulation(float /* deltaTime */) {
}

void ParticleSystem::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	// TODO [Implement] Figure out if its better to add/remove subscription dynamically on play/pause/remove
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void ParticleSystem::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
