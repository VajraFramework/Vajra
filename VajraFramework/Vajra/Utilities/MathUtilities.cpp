#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/MathUtilities.h"

const float PI = 3.14159265358979323846264f;

const glm::mat4 IDENTITY_MATRIX = glm::mat4(1.0f);
const glm::quat IDENTITY_QUATERNION = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

const glm::vec3 XAXIS = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 YAXIS = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 ZAXIS = glm::vec3(0.0f, 0.0f, 1.0f);
//
const glm::vec3 ZERO_VEC3 = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec4 ZERO_VEC4_POSITION  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 ZERO_VEC4_DIRECTION = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

// Interpolation Functions:
void lerp(float& destination, const float a, const float b, const float interp) {
	destination = a * (1 - interp) + b * interp;
}

void slerp(glm::quat& destination, const glm::quat a, const glm::quat b, float interp) {
	destination = glm::slerp(a, b, interp);
}

void catmullromerp(float& destination, const float p0, const float p1, const float p2, const float p3, const float interp) {
	destination = 0.5f * (
		(2*p1) +
		(-p0 + p2) * interp +
		(2*p0 - 5*p1 + 4*p2 - p3) * interp * interp +
		(-p0 + 3*p1 - 3*p2 +p3) * interp * interp * interp
		);
}

float Vec3Distance(glm::vec3 from, glm::vec3 to) {
	float dx = from.x - to.x;
	float dy = from.y - to.y;
	float dz = from.z - to.z;
	float dist = sqrt(dx * dx + dy * dy + dz * dz);
	return dist;
}

void lerp(glm::vec3& destination, const glm::vec3 a, const glm::vec3 b, const float interp) {
	lerp(destination.x, a.x, b.x, interp);
	lerp(destination.y, a.y, b.y, interp);
	lerp(destination.z, a.z, b.z, interp);
}
