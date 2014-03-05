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

#include <algorithm>

#define ROUNDING_ERROR 0.0001f

// Bit operations
#define BYTE_INDEX(x) (x / 8)
#define BIT_INDEX(x) (x % 8)
#define BYTE_WITH_BIT_SET(b) (0x01 << b)
#define BYTE_WITH_BIT_UNSET(b) (~(0x01 << b))
#define IS_BIT_SET_IN_BYTE(b,B) ((B & BYTE_WITH_BIT_SET(b)) != 0x00)
#define IS_BIT_SET_IN_BYTE_ARRAY(x,arr) IS_BIT_SET_IN_BYTE(BIT_INDEX(x), arr[BYTE_INDEX(x)])
#define SET_BIT_IN_BYTE(b,B) (B = (B | BYTE_WITH_BIT_SET(b)))
#define UNSET_BIT_IN_BYTE(b,B) (B = (B & BYTE_WITH_BIT_UNSET(b)))
#define SET_BIT_IN_BYTE_ARRAY(x,arr) (SET_BIT_IN_BYTE(BIT_INDEX(x), arr[BYTE_INDEX(x)]))
#define UNSET_BIT_IN_BYTE_ARRAY(x,arr) (UNSET_BIT_IN_BYTE(BIT_INDEX(x), arr[BYTE_INDEX(x)]))

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
enum InterpolationType_t {
	INTERPOLATION_TYPE_LINEAR,
	INTERPOLATION_TYPE_CUBIC,
	INTERPOLATION_TYPE_CUBIC_INVERSE,
	INTERPOLATION_TYPE_SINE,
	INTERPOLATION_TYPE_EXPONENTIAL,
	INTERPOLATION_TYPE_OVERSHOOT,
	INTERPOLATION_TYPE_PARABOLA,
	//
	INTERPOLATION_TYPE_NUM_INTERPOLATION_TYPES,
};

void lerp(float& destination, const float a, const float b, const float interp);
void cubicerp(float& destination, const float a, const float b, float interp, const float totalTime);
void cubicinverseerp(float& destination, const float a, const float b, float interp, const float totalTime);
void sineerp(float& destination, const float a, const float b, float interp, const float totalTime);
void exponentialerp(float& destination, const float a, const float b, float interp, const float totalTime);
void overshooterp(float& destination, const float a, const float b, float interp, const float totalTime);
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

float DistanceFromPlaneToPoint(glm::vec4 plane, glm::vec3 point);

// Vector functions
void lerp(glm::vec3& destination, const glm::vec3 a, const glm::vec3 b, const float interp);
void lerp(glm::vec4& destination, const glm::vec4 a, const glm::vec4 b, const float interp);

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

inline void clamp(float& out, float min, float max) {
	out = std::max(std::min(out, max), min);
}

inline void clamp(int& out, int min, int max) {
	out = std::max(std::min(out, max), min);
}

inline float randf() { return ((rand()%100) / 100.0f); }

#endif // MATHUTILITIES_H
