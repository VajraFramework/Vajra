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
	void InitParticleSystem(unsigned int numParticlesPerSecond, unsigned int maxNumParticles, float particleInitialVelocity, float particleFinalVelocity, float initialParticleSizePixels, float finalParticleSizePixels, float particleLifespanInSeconds);

protected:
	// @Override
	virtual void start();
	virtual void update();
	virtual void end();

private:
	void init();
	void destroy();

	void stepSimulation(float deltaTime);
	void spawnParticles(float deltaTime);
	void stepParticles (float deltaTime);
	void cleanupDeadParticles();

	glm::vec3* particlePositions;
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

	static unsigned int componentTypeId;

	friend class ParticleSystemRenderer;
};

#endif // PARTICLE_SYSTEM_H
