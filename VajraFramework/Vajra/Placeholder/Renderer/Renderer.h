#ifndef RENDERER_H
#define RENDERER_H

#ifdef PLATFORM_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif // PLATFORM_ANDROID

#ifdef PLATFORM_DESKTOP
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GL/glfw.h>
#endif // PLATFORM_DESKTOP

#ifdef PLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif // PLATFORM_IOS



#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtx/transform.hpp"
#include "Libraries/glm/gtc/type_ptr.hpp"
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
void setupMatrixes(float dt);
void renderFrame(float dt);
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


#endif // RENDERER_H