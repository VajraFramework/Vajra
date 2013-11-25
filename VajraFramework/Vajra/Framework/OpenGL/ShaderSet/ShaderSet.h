#ifndef SHADERSET_H
#define SHADERSET_H

#include "Vajra/Framework/OpenGL/ShaderHandles/ShaderHandles.h"
#include "Vajra/Utilities/OpenGLIncludes.h"

#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/type_ptr.hpp"

#include <string>

class ShaderSet {
public:
	ShaderSet(std::string inShaderSpecificationName);
	~ShaderSet();

	inline GLuint GetShaderProgram() { return this->shaderProgram; }

	inline GLint GetHandle(Shader_variable_variablename_id_t variablename_id) { return this->shaderHandles->GetShaderHandle(variablename_id); }
	inline bool  HasHandle(Shader_variable_variablename_id_t variablename_id) { return this->shaderHandles->HasShaderHandle(variablename_id); }


private:
	void init(std::string inShaderSpecificationName);
	void destroy();

	// Utility Functions:
	void createShaderProgram();
	void createVShader();
	void createFShader();
	//
	void createHandles();

	std::string vshaderSrcName;
	std::string fshaderSrcName;
	std::string shaderSpecificationName;
	//
	GLuint compiledVShader;
	GLuint compiledFShader;
	//
	std::vector<std::string> variablesUsed;

	GLuint shaderProgram;

	ShaderHandles* shaderHandles;
};

#endif // SHADERSET_H
