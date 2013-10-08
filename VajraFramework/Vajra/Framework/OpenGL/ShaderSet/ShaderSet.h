#ifndef SHADERSET_H
#define SHADERSET_H

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
#include "Libraries/glm/gtc/type_ptr.hpp"

#include <string>

class ShaderSet {
public:
	ShaderSet(std::string inVshaderName, std::string inFshaderName);
	~ShaderSet();

	inline GLuint GetShaderProgram() { return this->shaderProgram; }
	//
	inline GLuint GetPositionHandle() { return this->positionHandle; }
	inline GLuint GetTextureCoordsHandle() { return this->textureCoordsHandle; }
	//
	inline GLuint GetMVPMatrixHandle() { return this->mvpMatrixHandle; }
	inline void SetMVPMatrixHandle(glm::mat4& mvpMatrix);


private:
	std::string vshaderName;
	std::string fshaderName;

	GLuint shaderProgram;
	//
	GLuint positionHandle;
	GLuint textureCoordsHandle;
	//
	GLint mvpMatrixHandle;
};

// Inline Functions:
inline void ShaderSet::SetMVPMatrixHandle(glm::mat4& mvpMatrix) {
    glUniformMatrix4fv(this->GetMVPMatrixHandle(), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
}

#endif // SHADERSET_H
