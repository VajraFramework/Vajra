#ifndef LIGHT_SHADER_HANDLES_H
#define LIGHT_SHADER_HANDLES_H

#include "Vajra/Utilities/OpenGLIncludes.h"

class DLightShaderHandles {
public:
	DLightShaderHandles(GLuint shaderProgram);
	~DLightShaderHandles();

	inline GLint GetDirectionHandle() { return this->directionHandle; }
	//
	inline GLint GetAmbientColorHandle()  { return this->ambientColorHandle; }
	inline GLint GetDiffuseColorHandle()  { return this->diffuseColorHandle; }
	inline GLint GetSpecularColorHandle() { return this->specularColorHandle; }

private:
	void init(GLuint shaderProgram);
	void destroy();

	GLint directionHandle;
	GLint ambientColorHandle;
	GLint diffuseColorHandle;
	GLint specularColorHandle;
};

#endif // LIGHT_SHADER_HANDLES_H
