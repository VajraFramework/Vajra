#ifndef SHADER_HANDLES_H
#define SHADER_HANDLES_H

#include "Vajra/Framework/OpenGL/ShaderHandles/Declarations.h"
#include "Vajra/Utilities/OpenGLIncludes.h"
#include "Vajra/Utilities/Utilities.h"

#include <map>

// Forward Declarations:
class ShaderHandle;

class ShaderHandles {
public:
	~ShaderHandles();

	void AddShaderHandle(Shader_variable_qualifier_t qualifier, Shader_variable_datatype_t datatype, Shader_variable_variablename_id_t variablename_id);
	inline GLint GetShaderHandle(Shader_variable_variablename_id_t variablename_id);

private:
	ShaderHandles(GLuint shaderProgram_);

	void init(GLuint shaderProgram_);
	void destroy();

	GLuint shaderProgram;

	std::map<int /* shader handle id */, ShaderHandle*> shaderHandles;

	friend class ShaderSet;
};

#endif // SHADER_HANDLES_H

////////////////////////////////////////////////////////////////////////////////

class ShaderHandle {
public:
	ShaderHandle(Shader_variable_qualifier_t qualifier,
			     Shader_variable_datatype_t datatype,
			     Shader_variable_variablename_id_t variablename_id);

	Shader_variable_qualifier_t        qualifier;
	Shader_variable_datatype_t         datatype;
	Shader_variable_variablename_id_t  variablename_id;
	//
	GLint glHandle;

private:
};

////////////////////////////////////////////////////////////////////////////////

// Inline Functions:

GLint ShaderHandles::GetShaderHandle(Shader_variable_variablename_id_t variablename_id) {
	ASSERT(this->shaderHandles.find(variablename_id) != this->shaderHandles.end(), "Found shader variable for %d", variablename_id);
	GLint glHandle = this->shaderHandles[variablename_id]->glHandle;
	ASSERT(glHandle != -1, "Valid OpenGL shader variable handle");
	return glHandle;
}
