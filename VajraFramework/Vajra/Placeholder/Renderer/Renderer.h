#ifndef PLACEHOLDER_RENDERER_H
#define PLACEHOLDER_RENDERER_H

#include "Vajra/Utilities/OpenGLIncludes.h"

#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/type_ptr.hpp"
#include "Libraries/glm/gtx/transform.hpp"
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <map>
#include <string>

////////////////////////////////////////////////////////////////////////////////
// temp
bool setupGraphics(int w, int h);
bool renderFrame(); // If not enough time has passed since last frame
//
extern const char gVertexShader[];
extern const char gFragmentShader[];
//
extern glm::mat4 modelMatrix;
extern glm::mat4 viewMatrix;
extern glm::mat4 projectionMatrix;
extern GLint modelMatrixLocation;
extern GLint viewMatrixLocation;
extern GLint projectionMatrixLocation;
//
extern std::vector<unsigned int> indices;
extern GLuint elementbuffer;
//
extern GLuint gProgram;
extern GLuint gvPositionHandle;
//
////////////////////////////////////////////////////////////////////////////////


#endif // PLACEHOLDER_RENDERER_H
