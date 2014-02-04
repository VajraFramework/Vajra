#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/MathUtilities.h"
#include "Vajra/Utilities/Utilities.h"

#include "Libraries/glm/gtx/orthonormalize.hpp"
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

glm::quat QuaternionFromLookVectors(glm::vec3 requiredForward, glm::vec3 up /* = YAXIS */) {
	ASSERT(requiredForward != ZERO_VEC3, "QuaternionFromLookVectors: Forward vector can't be zero");
	ASSERT(up != ZERO_VEC3, "QuaternionFromLookVectors: Up vector can't be zero");

	glm::quat quaternion1 = IDENTITY_QUATERNION;
	glm::quat quaternion2 = IDENTITY_QUATERNION;

	{
		// Determine the rotation to align the forward vector
		glm::vec3 axis = glm::cross(ZAXIS, requiredForward);
		float angle;
		if (axis != ZERO_VEC3) {
			axis = glm::normalize(axis);
			angle = glm::angle(ZAXIS, requiredForward);
		}
		else {
			axis = YAXIS;
			angle = PI;
		}
		quaternion1 = glm::angleAxis(angle, axis);
	}
	if (up != YAXIS) {
		// Determine the rotation to align the up vector
		glm::vec3 axis = glm::cross(YAXIS, requiredForward);
		float angle;
		if (axis != ZERO_VEC3) {
			axis = glm::normalize(axis);
			angle = glm::angle(YAXIS, requiredForward);
		}
		else {
			axis = ZAXIS;
			angle = PI;
		}
		quaternion2 = glm::angleAxis(angle, axis);
	}

	return quaternion1 * quaternion2;
}

glm::vec3 QuaternionRightVector(glm::quat q) {
	glm::vec3 v;
	v.x = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	v.y = 2.0f * (q.x * q.y + q.w * q.z);
	v.z = 2.0f * (q.x * q.z - q.w * q.y);
	return glm::normalize(v);
}

glm::vec3 QuaternionUpVector(glm::quat q) {
	glm::vec3 v;
	v.x = 2.0f * (q.x * q.y - q.w * q.z);
	v.y = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
	v.z = 2.0f * (q.y * q.z + q.w * q.x);
	return glm::normalize(v);
}

glm::vec3 QuaternionForwardVector(glm::quat q) {
	glm::vec3 v;
	v.x = 2.0f * (q.x * q.z + q.w * q.y);
	v.y = 2.0f * (q.y * q.x - q.w * q.x);
	v.z = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	return glm::normalize(v);
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

void parabolaerp(glm::vec3& destination, const glm::vec3 a, const glm::vec3 b, const float parabola_a, const float interp) {
	float parabolaBaseLength = glm::distance(a, b);
	float maxHeightOfParabola = 4.0f * parabola_a * square(parabolaBaseLength / 2.0f);

	lerp(destination.x, a.x, b.x, interp);
	lerp(destination.z, a.z, b.z, interp);
	float parabola_y = (parabolaBaseLength * interp) - (parabolaBaseLength / 2.0f);
	destination.y = a.y + maxHeightOfParabola -
						  (4.0f * parabola_a *
						  square(parabola_y));

	// Adjust for when the parabolic path connects points that are at different heights -- like when jumping onto a ledge:
	destination.y += (b.y - a.y) * interp;
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
