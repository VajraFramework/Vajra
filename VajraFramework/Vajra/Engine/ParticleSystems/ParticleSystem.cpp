#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/ParticleSystems/ParticleSystem.h"

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

void ParticleSystem::init() {
}

void ParticleSystem::destroy() {
}
