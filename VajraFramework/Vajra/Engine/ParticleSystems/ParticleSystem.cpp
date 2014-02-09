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

#define MAXIMUM_TIME_BETWEEN_BATCH_SPAWNS_seconds 0.1f

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

void ParticleSystem::SetNumberOfParticles(unsigned int numParticlesPerSecond_, unsigned int maxNumParticles_) {
	this->numParticlesPerSecond = numParticlesPerSecond_;
	this->maxNumParticles = maxNumParticles_;
}

void ParticleSystem::SetParticleVelocity(float particleInitialVelocity_, float particleFinalVelocity_) {
	this->particleInitialVelocity = particleInitialVelocity_;
	this->particleFinalVelocity = particleFinalVelocity_;
}

void ParticleSystem::SetParticleSize(float initialParticleSizePixels_, float finalParticleSizePixels_) {
	this->initialParticleSizePixels = initialParticleSizePixels_;
	this->finalParticleSizePixels = finalParticleSizePixels_;
}

void ParticleSystem::SetParticleLifespan(float particleLifespanInSeconds_) {
	this->particleLifespanInSeconds = particleLifespanInSeconds_;
}

void ParticleSystem::SetParticleTexture(std::string pathToTexture_) {
	this->pathToTexture = pathToTexture_;
}

void ParticleSystem::SetParticleInitialColor(float r, float g, float b, float a) {
	this->particleInitialColor = glm::vec4(r, g, b, a);
}

void ParticleSystem::SetParticleFinalColor(float r, float g, float b, float a) {
	this->particleFinalColor = glm::vec4(r, g, b, a);
}

void ParticleSystem::InitParticleSystem() {

	// Create the pool of particles:
	unsigned int numParticlesToCreate = this->maxNumParticles + this->numParticlesPerSecond;
	for (unsigned int i = 0; i < numParticlesToCreate; ++i) {
		Particle* particle = new Particle();
		particle->initialVelocity = this->particleInitialVelocity; particle->finalVelocity = this->particleFinalVelocity;
		particle->initialSizePixels = this->initialParticleSizePixels; particle->finalSizePixels = this->finalParticleSizePixels;
		particle->totalLifespanInSeconds = this->particleLifespanInSeconds;
		particle->initialColor = this->particleInitialColor; particle->finalColor = this->particleFinalColor;
		//
		particle->reset();
		//
		this->deadParticles.push_back(particle);
	}

	// TODO [Hack] Figure this out better
	this->minimumTimeBetweenBatchSpawns = MAXIMUM_TIME_BETWEEN_BATCH_SPAWNS_seconds;

	// Init the shader attribute vectors for drawing:
	this->initShaderAttributeVectors();
}

void ParticleSystem::initShaderAttributeVectors() {
	this->numParticlesToDraw = this->maxNumParticles;

	this->particlePositions = new glm::vec3[this->maxNumParticles];
	this->particleSizes     = new float[this->maxNumParticles];
	this->particleColors    = new glm::vec4[this->maxNumParticles];

	for (unsigned int i = 0; i < this->numParticlesToDraw; ++i) {
		this->particlePositions[i] = glm::vec3(0.0f, 0.0f, 0.0f);
		this->particleSizes[i]     = 0.0f;
		this->particleColors[i]    = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	ENGINE->GetMessageHub()->SendPointcastMessage(MESSAGE_TYPE_PARTICLE_SYSTEM_INITED, this->GetObject()->GetId(), this->GetObject()->GetId());

	this->updateShaderAttributeVectors();
}

void ParticleSystem::updateShaderAttributeVectors() {
	// Copy the updated positions of all the alive particles:
	unsigned int particleIdx = 0;
	for (Particle* particle : this->aliveParticles) {
		// Copy the particles in reverse order for transperancy reasons:
		this->particlePositions[this->numParticlesToDraw - particleIdx - 1] = particle->position;
		this->particleSizes[this->numParticlesToDraw - particleIdx - 1]     = particle->size_in_pixels;
		this->particleColors[this->numParticlesToDraw - particleIdx - 1]    = particle->color;
		++particleIdx;
	}

	// Reset the rest:
	for (unsigned int i = particleIdx; i < this->maxNumParticles; ++i) {
		this->particlePositions[this->numParticlesToDraw - i - 1] = glm::vec3(0.0f, 0.0f, 0.0f);
		this->particleSizes[this->numParticlesToDraw - i - 1]     = 0.0f;
		this->particleColors[this->numParticlesToDraw - i - 1]    = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
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

	this->particlePositions = nullptr;
	this->particleSizes = nullptr;
	this->particleColors = nullptr;

	this->timeSinceLastBatchSpawn = 10000.0f;
	this->minimumTimeBetweenBatchSpawns = 0.0f;

	// Assign default values for all properties:
	this->numParticlesPerSecond           = 10.0f;
	this->maxNumParticles                 = 50.0f;
	this->particleInitialVelocity         = 0.1f;
	this->particleFinalVelocity           = 0.01f;
	this->initialParticleSizePixels       = 16;
	this->finalParticleSizePixels         = 32;
	this->particleLifespanInSeconds       = 3.0f;
	this->particleInitialColor            = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->particleFinalColor              = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// TODO [Implement] Figure out if its better to add/remove subscription dynamically on play/pause/remove
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void ParticleSystem::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
