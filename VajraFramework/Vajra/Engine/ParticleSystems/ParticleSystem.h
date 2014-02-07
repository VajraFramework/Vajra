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

private:
	void init();
	void destroy();

	glm::vec3* particlePositions;
	//
	std::vector<Particle*> aliveParticles;
	std::vector<Particle*> deadParticles;

	static unsigned int componentTypeId;
};

#endif // PARTICLE_SYSTEM_H
