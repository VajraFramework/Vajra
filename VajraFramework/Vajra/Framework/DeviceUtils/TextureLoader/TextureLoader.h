#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>

GLint loadGLTextureFromPNG(const char *name);

#endif // TEXTURELOADER_H