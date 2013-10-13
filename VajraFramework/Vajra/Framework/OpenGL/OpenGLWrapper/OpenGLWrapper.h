#ifndef OPENGLWRAPPER_H
#define OPENGLWRAPPER_H

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

#include "Vajra/Common/Objects/Object.h"

#include <map>
#include <string>

// Forward Declarations:
class ShaderSet;

class OpenGLWrapper : public Object {
public:
	~OpenGLWrapper();
	
	void PrintGLVersion();
	
	void CreateShaderSet(std::string shaderName, \
		std::string vshaderName, std::string fshaderName);
	//
	void SetCurrentShaderSet(std::string shaderName);
	inline ShaderSet* GetCurrentShaderSet() { return this->currentShaderSet; }

private:
	OpenGLWrapper();
	void init();
	void destroy();

	std::map<std::string /* shaderName */, ShaderSet*> shaderSets;
	ShaderSet *currentShaderSet;

	GLuint positionHandle;
	GLuint textureCoordsHandle;

	friend class Framework;
};

#endif // OPENGLWRAPPER_H
