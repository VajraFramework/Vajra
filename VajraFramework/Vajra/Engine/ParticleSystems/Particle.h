#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vajra/Utilities/MathUtilities.h"

#include "Libraries/glm/glm.hpp"

struct Particle {
public:
	Particle();

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 color;
	float size_in_pixels;
	float life_remaining_in_seconds;
	//
	float initialVelocity;
	float finalVelocity;
	//
	float initialSizePixels;
	float finalSizePixels;
	//
	glm::vec4 initialColor;
	glm::vec4 finalColor;
	//
	float totalLifespanInSeconds;

	void reset();
	void stepSimulation(float deltaTime);

private:
};

#endif // PARTICLE_H
