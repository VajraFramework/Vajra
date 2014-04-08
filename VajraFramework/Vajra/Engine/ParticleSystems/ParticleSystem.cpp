#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
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

void ParticleSystem::SetParticleSpeed(float particleInitialSpeed_, float particleFinalSpeed_) {
	this->particleInitialSpeed = particleInitialSpeed_;
	this->particleFinalSpeed = particleFinalSpeed_;
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

void ParticleSystem::SetParticleVelocityDirection(float x, float y, float z) {
	this->particleVelocityDirection = glm::vec3(x, y, z);
	ASSERT(this->particleVelocityDirection != ZERO_VEC3, "Particle direction not zero vector");
}

void ParticleSystem::SetEmissionVolume(std::string emissionVolumeType_, float radius_x, float radius_y, float radius_z) {
	if (emissionVolumeType_ == "point") {
		this->emissionVolumeType = EMISSION_VOLUME_TYPE_POINT;
	} else if (emissionVolumeType_ == "square") {
		this->emissionVolumeType = EMISSION_VOLUME_TYPE_SQUARE;
	} else if (emissionVolumeType_ == "circle_circle") {
		this->emissionVolumeType = EMISSION_VOLUME_TYPE_CIRCLE_CIRCLE;
	} else if (emissionVolumeType_ == "circle_surface") {
		this->emissionVolumeType = EMISSION_VOLUME_TYPE_CIRCLE_SURFACE;
	} else if (emissionVolumeType_ == "ellipsoid") {
		this->emissionVolumeType = EMISSION_VOLUME_TYPE_ELLIPSOID;
	}

	this->emission_volume_radius_x = radius_x;
	this->emission_volume_radius_y = radius_y;
	this->emission_volume_radius_z = radius_z;
}

void ParticleSystem::SetParticleVelocityDirectionRandomness(float randomness) {
	clamp(randomness, 0.0f, 1.0f);
	this->particleVelocityDirectionRandomness = randomness;
}

void ParticleSystem::SetAccelerationAmount(float accelerationAmount_) {
	this->accelerationAmount = accelerationAmount_;
}

void ParticleSystem::SetAccelerationDirection(float x, float y, float z) {
	this->accelerationDirection = glm::vec3(x, y, z);
	ASSERT(this->accelerationDirection != ZERO_VEC3, "Particle acceleration not zero vector");
}

void ParticleSystem::SetOverallLifespan(float overallLifespanInSeconds_) {
	this->overallLifespanInSeconds = overallLifespanInSeconds_;
}

void ParticleSystem::SetSimulateInWorldSpace(bool simulateInWorldSpace) {
	this->inWorldSpace = simulateInWorldSpace;
}

void ParticleSystem::SetName(std::string name_) {
	this->name = name_;
}

void ParticleSystem::InitParticleSystem() {

	// Create the pool of particles:
	unsigned int numParticlesToCreate = this->maxNumParticles + this->numParticlesPerSecond;
	for (unsigned int i = 0; i < numParticlesToCreate; ++i) {
		Particle* particle = new Particle();
		particle->initialSpeed = this->particleInitialSpeed; particle->finalSpeed = this->particleFinalSpeed;
		particle->initialSizePixels = this->initialParticleSizePixels; particle->finalSizePixels = this->finalParticleSizePixels;
		particle->totalLifespanInSeconds = this->particleLifespanInSeconds;
		particle->initialColor = this->particleInitialColor; particle->finalColor = this->particleFinalColor;
		//
		particle->reset(this->emissionVolumeType, this->emission_volume_radius_x, this->emission_volume_radius_y, this->emission_volume_radius_z, this->particleVelocityDirection, this->particleVelocityDirectionRandomness, this->inWorldSpace ? this->gameObjectRef->GetTransform()->GetPositionWorld() : ZERO_VEC3, this->inWorldSpace ? this->gameObjectRef->GetTransform()->GetOrientationWorld() : IDENTITY_QUATERNION);
		//
		this->dormantParticles.push_back(particle);
	}

	// TODO [Hack] Figure this out better
	this->minimumTimeBetweenBatchSpawns = MAXIMUM_TIME_BETWEEN_BATCH_SPAWNS_seconds;

	// Init the shader attribute vectors for drawing:
	this->initShaderAttributeVectors();

	this->isInited = true;
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
	if (this->isPlaying) {

		this->currentOverallLifespanInSeconds += deltaTime;

		this->spawnParticles(deltaTime);
		this->stepParticles (deltaTime);
		//
		this->cleanupDeadParticles();

		if (!this->isLooping) {
			if (this->aliveParticles.empty() || this->currentOverallLifespanInSeconds >= this->overallLifespanInSeconds) {
				this->raiseSpentEvent();
				this->reclaimDeadParticles();
				this->isPlaying = false;
				this->currentOverallLifespanInSeconds = 0.0f;
			}
		}

		this->updateShaderAttributeVectors();
	}
}

void ParticleSystem::raiseSpentEvent() {
	MessageChunk messageChunk = ENGINE->GetMessageHub()->GetOneFreeMessage();
	messageChunk->messageData.s = this->name;
	messageChunk->SetMessageType(MESSAGE_TYPE_PARTICLE_SYSTEM_SPENT);
	ENGINE->GetMessageHub()->SendPointcastMessage(messageChunk, this->GetObject()->GetId(), this->GetObject()->GetId());
}

void ParticleSystem::spawnParticles(float deltaTime) {
	float temp_timeSinceLastBatchSpawn = this->timeSinceLastBatchSpawn + deltaTime;
	this->timeSinceLastBatchSpawn = temp_timeSinceLastBatchSpawn;
	if (temp_timeSinceLastBatchSpawn <= this->minimumTimeBetweenBatchSpawns) {
		return;
	}

	// Spawn new particles:
	unsigned int numParticlesToAdd = ceil(this->numParticlesPerSecond * temp_timeSinceLastBatchSpawn);
	if (this->dormantParticles.size() < numParticlesToAdd) {
		numParticlesToAdd = this->dormantParticles.size();
	}
	numParticlesToAdd = std::min((unsigned int)(this->maxNumParticles - this->aliveParticles.size()), (unsigned int)numParticlesToAdd);
	std::deque<Particle*> particlesToAdd;
	for (unsigned int i = 0; i < numParticlesToAdd; ++i) {
		particlesToAdd.push_back(this->dormantParticles.front());
		this->dormantParticles.pop_front();
	}
	while (!particlesToAdd.empty()) {
		Particle* particle = particlesToAdd.front();
		particle->reset(this->emissionVolumeType, this->emission_volume_radius_x, this->emission_volume_radius_y, this->emission_volume_radius_z, this->particleVelocityDirection, this->particleVelocityDirectionRandomness, this->inWorldSpace ? this->gameObjectRef->GetTransform()->GetPositionWorld() : ZERO_VEC3, this->inWorldSpace ? this->gameObjectRef->GetTransform()->GetOrientationWorld() : IDENTITY_QUATERNION);
		particlesToAdd.pop_front();
		this->aliveParticles.push_back(particle);
	}

	this->timeSinceLastBatchSpawn = 0.0f;
}

void ParticleSystem::stepParticles(float deltaTime) {
	// Step all alive particles through the simulation:
	for (Particle* particle : this->aliveParticles) {
		particle->stepSimulation(deltaTime, this->accelerationDirection, this->accelerationAmount);
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

	std::list<Particle*>* particle_list_to_cleanup_into;
	if (this->isLooping) {
		particle_list_to_cleanup_into = &this->dormantParticles;
	} else {
		particle_list_to_cleanup_into = &this->deadParticles;
	}
	while (!particlesToCleanup.empty()) {
		particle_list_to_cleanup_into->push_back(particlesToCleanup.front());
		particlesToCleanup.pop_front();
	}
}

void ParticleSystem::reclaimDeadParticles() {
	while (!this->deadParticles.empty()) {
		Particle* particle = this->deadParticles.front();
		particle->reset(this->emissionVolumeType, this->emission_volume_radius_x, this->emission_volume_radius_y, this->emission_volume_radius_z, this->particleVelocityDirection, this->particleVelocityDirectionRandomness, this->inWorldSpace ? this->gameObjectRef->GetTransform()->GetPositionWorld() : ZERO_VEC3, this->inWorldSpace ? this->gameObjectRef->GetTransform()->GetOrientationWorld() : IDENTITY_QUATERNION);
		this->deadParticles.pop_front();
		this->dormantParticles.push_back(particle);
	}
	// TODO [Hack] Make it so that the next time Play() is called we are ready to spawn particles
	this->timeSinceLastBatchSpawn = MAXIMUM_TIME_BETWEEN_BATCH_SPAWNS_seconds + 1.0f;
}

void ParticleSystem::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->gameObjectRef = (GameObject*)this->GetObject();
	ASSERT(this->gameObjectRef->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");

	this->particlePositions = nullptr;
	this->particleSizes = nullptr;
	this->particleColors = nullptr;

	this->timeSinceLastBatchSpawn = 10000.0f;
	this->minimumTimeBetweenBatchSpawns = 0.0f;

	// Assign default values for all properties:
	this->inWorldSpace                    = false;
	this->emissionVolumeType              = EMISSION_VOLUME_TYPE_POINT;
	this->emission_volume_radius_x        = 0.0f;
	this->emission_volume_radius_y        = 0.0f;
	this->emission_volume_radius_z        = 0.0f;
	//
	this->numParticlesPerSecond           = 10.0f;
	this->maxNumParticles                 = 50.0f;
	this->particleInitialSpeed            = 0.1f;
	this->particleFinalSpeed              = 0.01f;
	this->initialParticleSizePixels       = 16;
	this->finalParticleSizePixels         = 32;
	this->particleLifespanInSeconds       = 3.0f;
	this->particleInitialColor            = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->particleFinalColor              = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->particleVelocityDirection       = YAXIS;
	this->particleVelocityDirectionRandomness       = 1.0f;
	this->accelerationAmount              = 0.0f;
	this->accelerationDirection           = -1.0f * YAXIS;
	//
	this->overallLifespanInSeconds        = 1000.0f;
	this->currentOverallLifespanInSeconds = 0.0f;

	this->isInited  = false;
	this->isPlaying = false;
	this->isLooping = true;

	// TODO [Implement] Figure out if its better to add/remove subscription dynamically on play/pause/remove
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void ParticleSystem::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void ParticleSystem::Play() {
	ASSERT(this->isInited == true, "ParticleSystem has been inited");
	this->isPlaying = true;
}

void ParticleSystem::Pause() {
	this->isPlaying = false;
}

void ParticleSystem::SetLooping(bool looping) {
	this->isLooping = looping;
}

