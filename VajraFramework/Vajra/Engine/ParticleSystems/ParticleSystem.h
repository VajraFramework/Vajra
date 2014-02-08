#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Vajra/Common/Components/Component.h"
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
	void SetParticleVelocity(float particleInitialVelocity, float particleFinalVelocity);
	//[[PROPERTY]]//
	void SetParticleSize(float initialParticleSizePixels, float finalParticleSizePixels);
	//[[PROPERTY]]//
	void SetParticleLifespan(float particleLifespanInSeconds);
	//[[PROPERTY]]//
	void SetParticleTexture(std::string pathToTexture_);
	//[[PROPERTY]]//
	void InitParticleSystem();

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

	// Functions required to expose the particle attribute vectors for drawing:
	inline unsigned int getNumParticlesToDraw() { return this->numParticlesToDraw; }
	inline glm::vec3* getParticlePositionsForDrawing()  { return this->particlePositions;  }
	inline float* getParticleSizesForDrawing()  { return this->particleSizes;  }

	std::string getPathToTexture() { return this->pathToTexture; }

	// Shader attribute vectors for drawing:
	unsigned int numParticlesToDraw;
	glm::vec3* particlePositions;
	float* particleSizes;
	//
	std::list<Particle*> aliveParticles;
	std::list<Particle*> deadParticles;

	unsigned int numParticlesPerSecond;
	unsigned int maxNumParticles;
	float particleInitialVelocity;
	float particleFinalVelocity;
	float initialParticleSizePixels;
	float finalParticleSizePixels;
	float particleLifespanInSeconds;
	std::string pathToTexture;

	float timeSinceLastBatchSpawn;
	float minimumTimeBetweenBatchSpawns;

	static unsigned int componentTypeId;

	friend class ParticleSystemRenderer;
};

#endif // PARTICLE_SYSTEM_H
