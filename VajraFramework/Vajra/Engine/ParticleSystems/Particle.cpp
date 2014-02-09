#include "Vajra/Engine/ParticleSystems/Particle.h"

#include "Libraries/glm/gtc/matrix_inverse.hpp"
#include "Libraries/glm/gtx/vector_angle.hpp"

#define DEFAULT_PARTICLE_SIZE_IN_PIXELS 16
#define DEFAULT_PARTICLE_INITIAL_LIFE_IN_SECONDS 5.0f

Particle::Particle() {
	this->size_in_pixels = DEFAULT_PARTICLE_SIZE_IN_PIXELS;
	this->life_remaining_in_seconds = DEFAULT_PARTICLE_INITIAL_LIFE_IN_SECONDS;
	this->color = this->initialColor = this->finalColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


void Particle::reset(glm::vec3 direction, float randomness) {
	// TODO [Implement] Come up with a random velocity
	float maxAngleToPerturbBy = 2.0f * PI * randomness;
	direction = glm::normalize(direction);
	direction = glm::rotate(direction, randf() * maxAngleToPerturbBy - maxAngleToPerturbBy/2.0f, XAXIS);
	direction = glm::rotate(direction, randf() * maxAngleToPerturbBy - maxAngleToPerturbBy/2.0f, YAXIS);
	direction = glm::rotate(direction, randf() * maxAngleToPerturbBy - maxAngleToPerturbBy/2.0f, ZAXIS);

	this->velocity = direction;

	this->position = ZERO_VEC3;
	this->size_in_pixels = this->initialSizePixels;
	this->life_remaining_in_seconds = this->totalLifespanInSeconds;
	this->color = this->initialColor;
}

void Particle::stepSimulation(float deltaTime, glm::vec3 accelerationDirection, float accelerationAmount) {
	this->life_remaining_in_seconds -= deltaTime;

	float interp = (this->totalLifespanInSeconds - this->life_remaining_in_seconds) / this->totalLifespanInSeconds;

	float currentVelocity_scalar;
	lerp(currentVelocity_scalar, this->initialSpeed, this->finalSpeed, interp);
	this->velocity += glm::normalize(accelerationDirection) * accelerationAmount * deltaTime;
	this->velocity = glm::normalize(this->velocity) * currentVelocity_scalar;

	lerp(this->size_in_pixels, this->initialSizePixels, this->finalSizePixels, interp);

	lerp(this->color, this->initialColor, this->finalColor, interp);

	this->position += (this->velocity * deltaTime);
}
