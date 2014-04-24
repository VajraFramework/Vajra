#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/ParticleSystems/Definitions.h"
#include "Vajra/Utilities/MathUtilities.h"

#include "Libraries/glm/glm.hpp"

#include <list>
#include <vector>

// Forward Declarations:
class Object;
class Particle;

//[[COMPONENT]]//
class ParticleSystem : public Component {
public:
	ParticleSystem();
	ParticleSystem(Object* object_);
	~ParticleSystem();

	static inline unsigned int GetTypeId() { return componentTypeId; }

	//[[PROPERTY]]//
	void SetNumberOfParticles(unsigned int numParticlesPerSecond_, unsigned int maxNumParticles);
	//[[PROPERTY]]//
	void SetParticleSpeed(float particleInitialSpeed, float particleFinalSpeed);
	//[[PROPERTY]]//
	void SetParticleSize(float initialParticleSizePixels, float finalParticleSizePixels);
	//[[PROPERTY]]//
	void SetParticleLifespan(float particleLifespanInSeconds);
	//[[PROPERTY]]//
	void SetParticleTexture(std::string pathToTexture_);
	//[[PROPERTY]]//
	void SetParticleInitialColor(float r, float g, float b, float a);
	//[[PROPERTY]]//
	void SetParticleFinalColor(float r, float g, float b, float a);
	//[[PROPERTY]]//
	void SetParticleVelocityDirection(float x, float y, float z);
	//[[PROPERTY]]//
	void SetParticleVelocityDirectionRandomness(float randomness);
	//[[PROPERTY]]//
	void SetAccelerationAmount(float accelerationAmount_);
	//[[PROPERTY]]//
	void SetAccelerationDirection(float x, float y, float z);
	//[[PROPERTY]]//
	void SetEmissionVolume(std::string emissionVolumeType_, float radius_x, float radius_y, float radius_z);
	//[[PROPERTY]]//
	void SetOverallLifespan(float overallLifespanInSeconds_);
	//[[PROPERTY]]//
	void SetSimulateInWorldSpace(bool simulateInWorldSpace);
	//[[PROPERTY]]//
	void SetName(std::string name_);
	//[[PROPERTY]]//
	void InitParticleSystem();
	//[[PROPERTY]]//
	void SetIsOneShot(bool isOneShot_);

	//[[PROPERTY]]//
	void Play();
	//[[PROPERTY]]//
	void Pause();
	//[[PROPERTY]]//
	void Stop();
	//[[PROPERTY]]//
	void SetLooping(bool looping);

	inline bool GetIsPlaying() { return this->isPlaying; }

protected:
	// @Override
	virtual void start();
	virtual void update();
	virtual void end();

private:
	void init();
	void destroy();

	void initShaderAttributeVectors();
	void updateShaderAttributeVectors();

	void stepSimulation(float deltaTime);
	void spawnParticles(float deltaTime);
	void stepParticles (float deltaTime);
	void cleanupDeadParticles();
	void reclaimDeadParticles();
	//
	void raiseSpentEvent();

	// Functions required to expose the particle attribute vectors for drawing:
	inline unsigned int getNumParticlesToDraw() { return this->numParticlesToDraw; }
	inline glm::vec3* getParticlePositionsForDrawing()  { return this->particlePositions;  }
	inline float* getParticleSizesForDrawing()  { return this->particleSizes;  }
	inline glm::vec4* getParticleColorsForDrawing()  { return this->particleColors;  }

	std::string getPathToTexture() { return this->pathToTexture; }

	// Shader attribute vectors for drawing:
	unsigned int numParticlesToDraw;
	glm::vec3* particlePositions;
	float* particleSizes;
	glm::vec4* particleColors;
	//
	std::list<Particle*> aliveParticles;
	std::list<Particle*> deadParticles;
	std::list<Particle*> dormantParticles;

	std::string name;
	bool inWorldSpace;
	unsigned int numParticlesPerSecond;
	unsigned int maxNumParticles;
	float particleInitialSpeed;
	float particleFinalSpeed;
	float initialParticleSizePixels;
	float finalParticleSizePixels;
	float particleLifespanInSeconds;
	std::string pathToTexture;
	glm::vec4 particleInitialColor;
	glm::vec4 particleFinalColor;
	glm::vec3 particleVelocityDirection;
	float particleVelocityDirectionRandomness;
	float accelerationAmount;
	glm::vec3 accelerationDirection;
	EmissionVolumeType_t emissionVolumeType;
	float emission_volume_radius_x;
	float emission_volume_radius_y;
	float emission_volume_radius_z;
	//
	float overallLifespanInSeconds;
	float currentOverallLifespanInSeconds;
	bool isOneShot;

	float timeSinceLastBatchSpawn;
	float minimumTimeBetweenBatchSpawns;

	bool isInited;
	bool isPlaying;
	bool isLooping;

	GameObject* gameObjectRef;

	static unsigned int componentTypeId;

	friend class ParticleSystemRenderer;
};

#endif // PARTICLE_SYSTEM_H
