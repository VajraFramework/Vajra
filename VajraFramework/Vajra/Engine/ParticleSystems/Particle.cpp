#include "Vajra/Engine/ParticleSystems/Particle.h"

#define DEFAULT_PARTICLE_SIZE_IN_PIXELS 16
#define DEFAULT_PARTICLE_INITIAL_LIFE_IN_SECONDS 5.0f

Particle::Particle() {
	this->size_in_pixels = DEFAULT_PARTICLE_SIZE_IN_PIXELS;
	this->life_remaining_in_seconds = DEFAULT_PARTICLE_INITIAL_LIFE_IN_SECONDS;
	this->color = this->initialColor = this->finalColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Particle::reset() {
	// TODO [Implement] Come up with a random velocity
	this->velocity = glm::vec3(rand()%100 / 100.0f - 0.5f, rand()%100 / 100.0f - 0.5f, rand()%100 / 100.0f - 0.5f);

	this->position = ZERO_VEC3;
	this->size_in_pixels = this->initialSizePixels;
	this->life_remaining_in_seconds = this->totalLifespanInSeconds;
	this->color = this->initialColor;
}

void Particle::stepSimulation(float deltaTime) {
	this->life_remaining_in_seconds -= deltaTime;

	float interp = (this->totalLifespanInSeconds - this->life_remaining_in_seconds) / this->totalLifespanInSeconds;

	float currentVelocity_scalar;
	lerp(currentVelocity_scalar, this->initialVelocity, this->finalVelocity, interp);
	this->velocity = glm::normalize(this->velocity) * currentVelocity_scalar;

	lerp(this->size_in_pixels, this->initialSizePixels, this->finalSizePixels, interp);

	lerp(this->color, this->initialColor, this->finalColor, interp);

	this->position += (this->velocity * deltaTime);
}
