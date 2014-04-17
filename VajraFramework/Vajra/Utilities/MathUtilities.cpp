#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Core/Framework.h"
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
	VERIFY(requiredForward != ZERO_VEC3, "QuaternionFromLookVectors: Forward vector can't be zero");
	VERIFY(up != ZERO_VEC3, "QuaternionFromLookVectors: Up vector can't be zero");

	glm::vec3 quatForward = glm::normalize(requiredForward);
	glm::vec3 quatUp = glm::orthonormalize(up, quatForward);
	glm::vec3 quatRight = glm::normalize(glm::cross(quatUp, quatForward));

	float m00 = quatRight.x;
	float m01 = quatRight.y;
	float m02 = quatRight.z;
	float m10 = quatUp.x;
	float m11 = quatUp.y;
	float m12 = quatUp.z;
	float m20 = quatForward.x;
	float m21 = quatForward.y;
	float m22 = quatForward.z;

	glm::quat finalQuat;
	float num8 = (m00 + m11) + m22;
	if (num8 > 0.0f) {
		float num = sqrt(num8 + 1.0f);
		finalQuat.w = num * 0.5f;
		num = 0.5f / num;
		finalQuat.x = (m12 - m21) * num;
		finalQuat.y = (m20 - m02) * num;
		finalQuat.z = (m01 - m10) * num;
		return finalQuat;
	}
	if ((m00 >= m11) && (m00 >= m22)) {
		float num7 = sqrt(((1.0f + m00) - m11) - m22);
		float num4 = 0.5f / num7;
		finalQuat.x = 0.5f * num7;
		finalQuat.y = (m01 + m10) * num4;
		finalQuat.z = (m02 + m20) * num4;
		finalQuat.w = (m12 - m21) * num4;
		return finalQuat;
	}
	if (m11 > m22) {
		float num6 = sqrt(((1.0f + m11) - m00) - m22);
		float num3 = 0.5f / num6;
		finalQuat.x = (m10 + m01) * num3;
		finalQuat.y = 0.5f * num6;
		finalQuat.z = (m21 + m12) * num3;
		finalQuat.w = (m20 - m02) * num3;
		return finalQuat;
	}
	float num5 = sqrt(((1.0f + m22) - m00) - m11);
	float num2 = 0.5f / num5;
	finalQuat.x = (m20 + m02) * num2;
	finalQuat.y = (m21 + m12) * num2;
	finalQuat.z = 0.5f * num5;
	finalQuat.w = (m01 - m10) * num2;
	return finalQuat;
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
	v.y = 2.0f * (q.y * q.z - q.w * q.x);
	v.z = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	return glm::normalize(v);
}

// Interpolation Functions:
void lerp(float& destination, const float a, const float b, const float interp) {
	destination = a + (b - a) * interp;
	clamp(destination, std::min(a, b), std::max(a, b));
}

void cubicerp(float& destination, const float a, const float b, float interp, const float totalTime) {
	interp /= (totalTime / 2.0f);
	if (interp < 1.0f) {
		destination = (b - a) / 2.0f * interp * interp * interp + a;
	} else {
		interp -= 2.0f;
		destination = (b - a) / 2.0f * (interp * interp * interp + 2.0f) + a;
	}
	clamp(destination, std::min(a, b), std::max(a, b));
}

void cubicinverseerp(float& destination, const float a, const float b, float interp, const float totalTime) {
	interp /= (totalTime / 2.0f);
	if (interp < 1) {
		interp -= 2.0f;
		destination = (b - a) / 2.0f * (interp * interp * interp + 2.0f) + a;
	} else {
		destination = (b - a) / 2.0f * interp * interp * interp + a;
	}
	clamp(destination, std::min(a, b), std::max(a, b));
}

void sineerp(float& destination, const float a, const float b, float interp, const float totalTime) {
	destination = -(b - a) / 2.0f * (cos(PI * interp / totalTime) - 1.0f) + a;
	clamp(destination, std::min(a, b), std::max(a, b));
}

void exponentialerp(float& destination, const float a, const float b, float interp, const float totalTime) {
	if (interp == 0) { destination = a; return; }
	if (interp >= totalTime) { destination = b; return; }
	interp /= (totalTime / 2.0f);
	if (interp < 1.0f) {
		destination = (b - a) / 2.0f * pow(2, 10.0f * (interp - 1)) + a;
	} else {
		destination = (b - a) / 2.0f * (-pow(2, -10.0f * (interp - 1)) + 2) + a;
	}
	clamp(destination, std::min(a, b), std::max(a, b));
}

void overshooterp(float& destination, const float a, const float b, float interp, const float totalTime) {
	float s = 1.70158f;
	interp /= (totalTime / 2.0f);
	if (interp < 1.0f) {
		s*=1.525f;
		destination = (b - a) / 2.0f * (interp * interp *(((s)+1) * interp - s)) + a;
	} else {
		float postFix = interp -= 2;
		s*=1.525f;
		destination = (b - a) / 2.0f *((postFix)* interp *(((s)+1)* interp + s) + 2) + a;
	}
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

bool isnanv(glm::vec3 v) {
	return (std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z));
}

bool isnanv(glm::vec4 v) {
	return (std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z) || std::isnan(v.w));
}

bool isinfv(glm::vec3 v) {
	return (std::isinf(v.x) || std::isinf(v.y) || std::isinf(v.z));
}

bool isinfv(glm::vec4 v) {
	return (std::isinf(v.x) || std::isinf(v.y) || std::isinf(v.z) || std::isinf(v.w));
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

void cubicerp        (glm::vec3& destination, const glm::vec3 a, const glm::vec3 b, float interp, const float totalTime) {
	cubicerp(destination.x, a.x, b.x, interp, totalTime);
	cubicerp(destination.y, a.y, b.y, interp, totalTime);
	cubicerp(destination.z, a.z, b.z, interp, totalTime);
}

void cubicinverseerp (glm::vec3& destination, const glm::vec3 a, const glm::vec3 b, float interp, const float totalTime) {
	cubicinverseerp(destination.x, a.x, b.x, interp, totalTime);
	cubicinverseerp(destination.y, a.y, b.y, interp, totalTime);
	cubicinverseerp(destination.z, a.z, b.z, interp, totalTime);
}

void sineerp         (glm::vec3& destination, const glm::vec3 a, const glm::vec3 b, float interp, const float totalTime) {
	sineerp(destination.x, a.x, b.x, interp, totalTime);
	sineerp(destination.y, a.y, b.y, interp, totalTime);
	sineerp(destination.z, a.z, b.z, interp, totalTime);
}

void exponentialerp  (glm::vec3& destination, const glm::vec3 a, const glm::vec3 b, float interp, const float totalTime) {
	exponentialerp(destination.x, a.x, b.x, interp, totalTime);
	exponentialerp(destination.y, a.y, b.y, interp, totalTime);
	exponentialerp(destination.z, a.z, b.z, interp, totalTime);
}

void overshooterp    (glm::vec3& destination, const glm::vec3 a, const glm::vec3 b, float interp, const float totalTime) {
	overshooterp(destination.x, a.x, b.x, interp, totalTime);
	overshooterp(destination.y, a.y, b.y, interp, totalTime);
	overshooterp(destination.z, a.z, b.z, interp, totalTime);
}

void lerp(glm::vec4& destination, const glm::vec4 a, const glm::vec4 b, const float interp) {
	lerp(destination.x, a.x, b.x, interp);
	lerp(destination.y, a.y, b.y, interp);
	lerp(destination.z, a.z, b.z, interp);
	lerp(destination.w, a.w, b.w, interp);
}

void cubicerp        (glm::vec4& destination, const glm::vec4 a, const glm::vec4 b, float interp, const float totalTime) {
	cubicerp(destination.x, a.x, b.x, interp, totalTime);
	cubicerp(destination.y, a.y, b.y, interp, totalTime);
	cubicerp(destination.z, a.z, b.z, interp, totalTime);
	cubicerp(destination.w, a.w, b.w, interp, totalTime);
}

void cubicinverseerp (glm::vec4& destination, const glm::vec4 a, const glm::vec4 b, float interp, const float totalTime) {
	cubicinverseerp(destination.x, a.x, b.x, interp, totalTime);
	cubicinverseerp(destination.y, a.y, b.y, interp, totalTime);
	cubicinverseerp(destination.z, a.z, b.z, interp, totalTime);
	cubicinverseerp(destination.w, a.w, b.w, interp, totalTime);
}

void sineerp         (glm::vec4& destination, const glm::vec4 a, const glm::vec4 b, float interp, const float totalTime) {
	sineerp(destination.x, a.x, b.x, interp, totalTime);
	sineerp(destination.y, a.y, b.y, interp, totalTime);
	sineerp(destination.z, a.z, b.z, interp, totalTime);
	sineerp(destination.w, a.w, b.w, interp, totalTime);
}

void exponentialerp  (glm::vec4& destination, const glm::vec4 a, const glm::vec4 b, float interp, const float totalTime) {
	exponentialerp(destination.x, a.x, b.x, interp, totalTime);
	exponentialerp(destination.y, a.y, b.y, interp, totalTime);
	exponentialerp(destination.z, a.z, b.z, interp, totalTime);
	exponentialerp(destination.w, a.w, b.w, interp, totalTime);
}

void overshooterp    (glm::vec4& destination, const glm::vec4 a, const glm::vec4 b, float interp, const float totalTime) {
	overshooterp(destination.x, a.x, b.x, interp, totalTime);
	overshooterp(destination.y, a.y, b.y, interp, totalTime);
	overshooterp(destination.z, a.z, b.z, interp, totalTime);
	overshooterp(destination.w, a.w, b.w, interp, totalTime);
}

void parabolaerp     (glm::vec4& destination, const glm::vec4 a, const glm::vec4 b, const float parabola_a, const float interp) {
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

	destination.w = a.w;
}



float DistanceFromPlaneToPoint(glm::vec4 plane, glm::vec3 point) {
	return (plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w);
}
