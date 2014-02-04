#ifndef MATHUTILITIES_H
#define MATHUTILITIES_H

/*
 * This file defines some useful symbols, macros, and functions
 * for math operations, and specifically for 3D math.
 * It assumes the use of glm as the math library.
 */

#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/quaternion.hpp"
#include "Libraries/glm/gtx/quaternion.hpp"
#include "Libraries/glm/gtx/transform.hpp"

#define ROUNDING_ERROR 0.0001f

extern const float PI;
#define inRadians * PI / 180.0f

extern const glm::mat4 IDENTITY_MATRIX;
extern const glm::quat IDENTITY_QUATERNION;

extern const glm::vec3 XAXIS;
extern const glm::vec3 YAXIS;
extern const glm::vec3 ZAXIS;
//
extern const glm::vec3 ZERO_VEC3;
extern const glm::vec4 ZERO_VEC4_POSITION;
extern const glm::vec4 ZERO_VEC4_DIRECTION;

glm::quat QuaternionFromLookVectors(glm::vec3 requiredForward, glm::vec3 up = YAXIS);

// Functions to extract basis vectors from a quaternion:
glm::vec3 QuaternionRightVector(glm::quat q);
glm::vec3 QuaternionUpVector(glm::quat q);
glm::vec3 QuaternionForwardVector(glm::quat q);

// Interpolation Functions:
void lerp(float& destination, const float a, const float b, const float interp);
void slerp(glm::quat& destination, const glm::quat a, const glm::quat b, float interp);
void catmullromerp(float& destination, const float p0, const float p1, const float p2, const float p3, const float interp);
void parabolaerp(glm::vec3& destination, const glm::vec3 a, const glm::vec3 b, const float parabola_a, const float interp);

// Simple intersection
struct Ray {
	glm::vec3 origin;
	glm::vec3 dir;
};

struct Plane {
	glm::vec3 origin;
	glm::vec3 normal;
};

bool rayPlaneIntersection(Ray& ray, Plane& plane, float& dist);

// Vector functions
void lerp(glm::vec3& destination, const glm::vec3 a, const glm::vec3 b, const float interp);

inline float square(float number) {
	return number * number;
}

inline double square(double number) {
	return number * number;
}

inline int square(int number) {
	return number * number;
}

inline unsigned int square(unsigned int number) {
	return number * number;
}

#endif // MATHUTILITIES_H
