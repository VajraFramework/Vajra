#ifndef SHADERSET_H
#define SHADERSET_H

#include "Libraries/glm/glm.hpp"
#include "Vajra/Utilities/OpenGLIncludes.h"

#include "Libraries/glm/gtc/type_ptr.hpp"

#include <string>

class ShaderSet {
public:
	ShaderSet(std::string inVshaderName, std::string inFshaderName);
	~ShaderSet();

	inline GLuint GetShaderProgram() { return this->shaderProgram; }
	//
	inline GLuint GetPositionHandle() { return this->positionHandle; }
	inline GLuint GetNormalHandle() { return this->normalHandle; }
	inline GLuint GetTextureCoordsHandle() { return this->textureCoordsHandle; }
	//
	inline GLuint GetMVPMatrixHandle() { return this->mvpMatrixHandle; }
	inline GLuint GetMitMatrixHandle() { return this->modelInverseTransposeMatrixHandle; }
	inline void SetMVPMatrixHandle(glm::mat4& mvpMatrix);
	inline void SetMitMatrixHandle(glm::mat4& mitMatrix);


private:
	std::string vshaderName;
	std::string fshaderName;

	GLuint shaderProgram;
	//
	GLuint positionHandle;
	GLuint normalHandle;
	GLuint textureCoordsHandle;
	//
	GLint mvpMatrixHandle;
	GLint modelInverseTransposeMatrixHandle;
};

// Inline Functions:

inline void ShaderSet::SetMVPMatrixHandle(glm::mat4& mvpMatrix) {
    glUniformMatrix4fv(this->GetMVPMatrixHandle(), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
}

inline void ShaderSet::SetMitMatrixHandle(glm::mat4& mitMatrix) {
    glUniformMatrix4fv(this->GetMitMatrixHandle(), 1, GL_FALSE, glm::value_ptr(mitMatrix));
}

#endif // SHADERSET_H
