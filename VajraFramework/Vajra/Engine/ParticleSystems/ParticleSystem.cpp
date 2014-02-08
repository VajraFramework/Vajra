#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/ParticleSystems/Particle.h"
#include "Vajra/Engine/ParticleSystems/ParticleSystem.h"
#include "Vajra/Engine/Timer/Timer.h"

#include <deque>

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

#define MAXIMUM_TIME_BETWEEN_BATCH_SPAWNS_seconds 0.25f

void ParticleSystem::InitParticleSystem(unsigned int numParticlesPerSecond_, unsigned int maxNumParticles_,
										float particleInitialVelocity_, float particleFinalVelocity_,
										float initialParticleSizePixels_, float finalParticleSizePixels_,
										float particleLifespanInSeconds_) {

	this->numParticlesPerSecond = numParticlesPerSecond_;
	this->maxNumParticles = maxNumParticles_;
	this->particleInitialVelocity = particleInitialVelocity_;
	this->particleFinalVelocity = particleFinalVelocity_;
	this->initialParticleSizePixels = initialParticleSizePixels_;
	this->finalParticleSizePixels = finalParticleSizePixels_;
	this->particleLifespanInSeconds = particleLifespanInSeconds_;

	// Create the pool of particles:
	unsigned int numParticlesToCreate = maxNumParticles_ + numParticlesPerSecond_;
	for (unsigned int i = 0; i < numParticlesToCreate; ++i) {
		Particle* particle = new Particle();
		particle->initialVelocity = particleInitialVelocity_; particle->finalVelocity = particleFinalVelocity_;
		particle->initialSizePixels = initialParticleSizePixels_; particle->finalSizePixels = finalParticleSizePixels_;
		particle->totalLifespanInSeconds = particleLifespanInSeconds_;
		//
		particle->reset();
		//
		this->deadParticles.push_back(particle);
	}

	this->minimumTimeBetweenBatchSpawns = MAXIMUM_TIME_BETWEEN_BATCH_SPAWNS_seconds;

	// Init the shader attribute vectors for drawing:
	this->initShaderAttributeVectors();
}

void ParticleSystem::initShaderAttributeVectors() {
	this->numParticlesToDraw = this->maxNumParticles;

	this->particlePositions = new glm::vec3[this->maxNumParticles];
	this->particleSizes     = new float[this->maxNumParticles];

	for (unsigned int i = 0; i < this->numParticlesToDraw; ++i) {
		this->particlePositions[i] = glm::vec3(0.0f, 0.0f, 0.0f);
		this->particleSizes[i]     = 0.0f;
	}

	ENGINE->GetMessageHub()->SendPointcastMessage(MESSAGE_TYPE_PARTICLE_SYSTEM_INITED, this->GetObject()->GetId(), this->GetObject()->GetId());

	this->updateShaderAttributeVectors();
}

void ParticleSystem::updateShaderAttributeVectors() {
	// Copy the updated positions of all the alive particles:
	unsigned int particleIdx = 0;
	for (Particle* particle : this->aliveParticles) {
		this->particlePositions[particleIdx] = particle->position;
		this->particleSizes[particleIdx]     = particle->size_in_pixels;
		++particleIdx;
	}

	// Reset the rest:
	for (unsigned int i = particleIdx; i < this->maxNumParticles; ++i) {
		this->particlePositions[i] = glm::vec3(0.0f, 0.0f, 0.0f);
		this->particleSizes[i]     = 0.0f;
	}

	ENGINE->GetMessageHub()->SendPointcastMessage(MESSAGE_TYPE_PARTICLE_SYSTEM_UPDATED, this->GetObject()->GetId(), this->GetObject()->GetId());
}

void ParticleSystem::stepSimulation(float deltaTime) {

	this->spawnParticles(deltaTime);
	this->stepParticles (deltaTime);
	this->cleanupDeadParticles();

	this->updateShaderAttributeVectors();
}

void ParticleSystem::spawnParticles(float deltaTime) {
	float temp_timeSinceLastBatchSpawn = this->timeSinceLastBatchSpawn + deltaTime;
	this->timeSinceLastBatchSpawn = temp_timeSinceLastBatchSpawn;
	if (temp_timeSinceLastBatchSpawn <= this->minimumTimeBetweenBatchSpawns) {
		return;
	}

	// Spawn new particles:
	unsigned int numParticlesToAdd = ceil(this->numParticlesPerSecond * temp_timeSinceLastBatchSpawn);
	if (this->deadParticles.size() < numParticlesToAdd) {
		numParticlesToAdd = this->deadParticles.size();
	}
	numParticlesToAdd = std::min((unsigned int)(this->maxNumParticles - this->aliveParticles.size()), (unsigned int)numParticlesToAdd);
	std::deque<Particle*> particlesToAdd;
	for (unsigned int i = 0; i < numParticlesToAdd; ++i) {
		particlesToAdd.push_back(this->deadParticles.front());
		this->deadParticles.pop_front();
	}
	while (!particlesToAdd.empty()) {
		Particle* particle = particlesToAdd.front();
		particle->reset();
		particlesToAdd.pop_front();
		this->aliveParticles.push_back(particle);
	}

	this->timeSinceLastBatchSpawn = 0.0f;
}

void ParticleSystem::stepParticles(float deltaTime) {
	// Step all alive particles through the simulation:
	for (Particle* particle : this->aliveParticles) {
		particle->stepSimulation(deltaTime);
		// TODO [Implement] Apply updated particle properties to big arrays
	}
}

void ParticleSystem::cleanupDeadParticles() {
	std::deque<Particle*> particlesToCleanup;
	for (auto it = this->aliveParticles.begin(); it != this->aliveParticles.end(); ++it) {
		Particle* particle = *it;
		if (particle->life_remaining_in_seconds <= 0.0f) {
			particlesToCleanup.push_back(particle);
			it = this->aliveParticles.erase(it);
			if (it == this->aliveParticles.end()) {
				break;
			}
		}
	}

	while (!particlesToCleanup.empty()) {
		this->deadParticles.push_back(particlesToCleanup.front());
		particlesToCleanup.pop_front();
	}
}

void ParticleSystem::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->timeSinceLastBatchSpawn = 10000.0f;
	this->minimumTimeBetweenBatchSpawns = 0.0f;

	// TODO [Implement] Figure out if its better to add/remove subscription dynamically on play/pause/remove
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void ParticleSystem::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

