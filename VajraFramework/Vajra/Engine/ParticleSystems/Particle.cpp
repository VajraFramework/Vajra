#include "Vajra/Engine/ParticleSystems/Particle.h"

#define DEFAULT_PARTICLE_SIZE_IN_PIXELS 16
#define DEFAULT_PARTICLE_INITIAL_LIFE_IN_SECONDS 5.0f

Particle::Particle() {
	this->size_in_pixels = DEFAULT_PARTICLE_SIZE_IN_PIXELS;
	this->life_remaining_in_seconds = DEFAULT_PARTICLE_INITIAL_LIFE_IN_SECONDS;
}
