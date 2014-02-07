#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vajra/Utilities/MathUtilities.h"

#include "Libraries/glm/glm.hpp"

struct Particle {
public:
	Particle();

	glm::vec3 position;
	glm::vec3 velocity;
	float size_in_pixels;

	float life_remaining_in_seconds;

private:
};

#endif // PARTICLE_H
