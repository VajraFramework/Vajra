#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/MathUtilities.h"

#include "Libraries/glm/gtx/vector_angle.hpp"

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

glm::quat LookRotation(glm::vec3 requiredForward, glm::vec3 up /* = YAXIS */) {
	glm::quat quaternion1 = IDENTITY_QUATERNION;
	glm::quat quaternion2 = IDENTITY_QUATERNION;

	{
		glm::vec3 crossProduct = glm::cross(ZAXIS, requiredForward);
		if (crossProduct != ZERO_VEC3) {
			crossProduct = glm::normalize(crossProduct);
		}
		else {
			crossProduct = up;
		}
		quaternion1 = glm::angleAxis(glm::angle(ZAXIS, requiredForward), crossProduct);
	}
	{
		if (up != YAXIS) {
			glm::vec3 crossProduct = glm::cross(YAXIS, up);
			crossProduct = glm::normalize(crossProduct);
			quaternion2 = glm::angleAxis(glm::angle(YAXIS, up), crossProduct);
		}
	}

	return quaternion1 * quaternion2;
}

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

bool rayPlaneIntersection(Ray& ray, Plane& plane, float& dist) {
	if(glm::dot(plane.normal, ray.dir) == 0) {
		return false;
	}
	glm::vec3 rayToPlane = plane.origin - ray.origin;
	dist = (glm::dot(plane.normal, rayToPlane) / glm::dot(plane.normal, ray.dir));
	return (dist >= 0);
}

void lerp(glm::vec3& destination, const glm::vec3 a, const glm::vec3 b, const float interp) {
	lerp(destination.x, a.x, b.x, interp);
	lerp(destination.y, a.y, b.y, interp);
	lerp(destination.z, a.z, b.z, interp);
}
