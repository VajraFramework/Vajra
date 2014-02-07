#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Utilities/MathUtilities.h"

#include "Libraries/glm/glm.hpp"

#include <vector>

// Forward Declarations:
class Object;
class Particle;

class ParticleSystem : public Component {
public:
	ParticleSystem();
	ParticleSystem(Object* object_);
	~ParticleSystem();

	static inline unsigned int GetTypeId() { return componentTypeId; }

protected:
	// @Override
	virtual void start();
	virtual void update();
	virtual void end();

private:
	void init();
	void destroy();

	void stepSimulation(float deltaTime);

	glm::vec3* particlePositions;
	//
	std::vector<Particle*> aliveParticles;
	std::vector<Particle*> deadParticles;

	static unsigned int componentTypeId;
};

#endif // PARTICLE_SYSTEM_H
