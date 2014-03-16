#ifndef OPENGL_INCLUDES_H
#define OPENGL_INCLUDES_H

#include <GL/glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#if 0
#ifdef PLATFORM_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif // PLATFORM_ANDROID

#ifdef PLATFORM_DESKTOP
// Include GLEW
#include <GL/glew.h>
// Include GLFW
// #include <GL/glfw.h>
#include <GLFW/glfw3.h>
#endif // PLATFORM_DESKTOP

#ifdef PLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif // PLATFORM_IOS
#endif

#endif // OPENGL_INCLUDES_H
