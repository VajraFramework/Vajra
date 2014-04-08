#include "Vajra/Engine/ParticleSystems/Particle.h"
#include "Vajra/Utilities/MathUtilities.h"
#include "Vajra/Utilities/Utilities.h"

#include "Libraries/glm/gtc/matrix_inverse.hpp"
#include "Libraries/glm/gtx/vector_angle.hpp"

#define DEFAULT_PARTICLE_SIZE_IN_PIXELS 16
#define DEFAULT_PARTICLE_INITIAL_LIFE_IN_SECONDS 5.0f

Particle::Particle() {
	this->size_in_pixels = DEFAULT_PARTICLE_SIZE_IN_PIXELS;
	this->life_remaining_in_seconds = DEFAULT_PARTICLE_INITIAL_LIFE_IN_SECONDS;
	this->color = this->initialColor = this->finalColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


void Particle::reset(EmissionVolumeType_t emissionVolumeType, float emission_radius_x, float emission_radius_y, float emission_radius_z, glm::vec3 direction, float randomness, glm::vec3 initialPosition, glm::quat initialDirectionOffset) {


	switch (emissionVolumeType) {
	case EMISSION_VOLUME_TYPE_POINT: {
		this->position = ZERO_VEC3;
		// TODO [Implement] Come up with a random velocity, maybe
		float maxAngleToPerturbBy = 2.0f * PI * randomness;
		direction = glm::normalize(direction);
		direction = glm::rotate(direction, randf() * maxAngleToPerturbBy - maxAngleToPerturbBy/2.0f, XAXIS);
		direction = glm::rotate(direction, randf() * maxAngleToPerturbBy - maxAngleToPerturbBy/2.0f, YAXIS);
		direction = glm::rotate(direction, randf() * maxAngleToPerturbBy - maxAngleToPerturbBy/2.0f, ZAXIS);
	} break;

	case EMISSION_VOLUME_TYPE_CIRCLE_SURFACE: {
		float theta = randf() * 2.0f * PI;
		position = glm::vec3(randf() * emission_radius_x * cos(theta), 0.0f, randf() * emission_radius_x * sin(theta));
		direction = YAXIS;
	} break;

	case EMISSION_VOLUME_TYPE_CIRCLE_CIRCLE: {
		float theta = randf() * 2.0f * PI;
		position = glm::vec3(emission_radius_x * cos(theta), 0.0f, emission_radius_x * sin(theta));
		direction = YAXIS;
	} break;

	case EMISSION_VOLUME_TYPE_ELLIPSOID: {
		float theta = randf() * 2.0f * PI;
		float psi = randf() * 2.0f * PI;
		float radius_at_theta = cos(theta);
		position = glm::vec3(radius_at_theta * cos(psi), sin(theta), radius_at_theta * sin(psi));
		position.x *= emission_radius_x;
		position.y *= emission_radius_y;
		position.z *= emission_radius_z;
		direction = this->position;
		direction = glm::normalize(direction);
	} break;

	case EMISSION_VOLUME_TYPE_SQUARE: {
		this->position = glm::vec3(randf() * 2.0f * emission_radius_x - emission_radius_x, 0.0f, randf() * 2.0f * emission_radius_z - emission_radius_z);
		direction = YAXIS;
	} break;

	default: {
		ASSERT(0, "Valid emission volume type %d", emissionVolumeType);
	} break;

	}
	this->position += initialPosition;

	this->velocity = glm::rotate(initialDirectionOffset, direction);


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
