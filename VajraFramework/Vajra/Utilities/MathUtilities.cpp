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
