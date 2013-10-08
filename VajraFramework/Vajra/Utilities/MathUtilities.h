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

extern const float PI;

extern const glm::mat4 IDENTITY_MATRIX;
extern const glm::quat IDENTITY_QUATERNION;

extern const glm::vec3 XAXIS;
extern const glm::vec3 YAXIS;
extern const glm::vec3 ZAXIS;
//
extern const glm::vec3 ZERO_VEC3;
extern const glm::vec4 ZERO_VEC4_POSITION;
extern const glm::vec4 ZERO_VEC4_DIRECTION;

#endif // MATHUTILITIES_H
