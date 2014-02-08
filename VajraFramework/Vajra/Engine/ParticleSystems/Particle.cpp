#include "Vajra/Engine/ParticleSystems/Particle.h"

#define DEFAULT_PARTICLE_SIZE_IN_PIXELS 16
#define DEFAULT_PARTICLE_INITIAL_LIFE_IN_SECONDS 5.0f

Particle::Particle() {
	this->size_in_pixels = DEFAULT_PARTICLE_SIZE_IN_PIXELS;
	this->life_remaining_in_seconds = DEFAULT_PARTICLE_INITIAL_LIFE_IN_SECONDS;
}

void Particle::reset() {
	// TODO [Implement] Come up with a random velocity
	this->velocity = YAXIS;
}

void Particle::stepSimulation(float deltaTime) {
	this->life_remaining_in_seconds -= deltaTime;

	float interp = (this->totalLifespanInSeconds - this->life_remaining_in_seconds) / this->totalLifespanInSeconds;

	float currentVelocity_scalar;
	lerp(currentVelocity_scalar, this->initialVelocity, this->finalVelocity, interp);
	this->velocity = glm::normalize(this->velocity) * currentVelocity_scalar;

	this->position += (this->velocity * deltaTime);
}
