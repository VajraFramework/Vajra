#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/OpenGL/ShaderHandles/ShaderHandles.h"
#include "Vajra/Framework/Logging/Logger.h"


inline ShaderHandle::ShaderHandle(Shader_variable_qualifier_t qualifier,
								  Shader_variable_datatype_t datatype,
								  Shader_variable_variablename_id_t variablename_id) {
	this->qualifier       = qualifier;
	this->datatype        = datatype;
	this->variablename_id = variablename_id;
}

////////////////////////////////////////////////////////////////////////////////

ShaderHandles::ShaderHandles(GLuint shaderProgram_) {
	this->init(shaderProgram_);
}

ShaderHandles::~ShaderHandles() {
	this->destroy();
}

void ShaderHandles::init(GLuint shaderProgram_) {
	this->shaderProgram = shaderProgram_;
}

void ShaderHandles::AddShaderHandle(Shader_variable_qualifier_t qualifier,
									Shader_variable_datatype_t datatype,
									Shader_variable_variablename_id_t variablename_id) {

	ShaderHandle* newShaderHandle = new ShaderHandle(qualifier, datatype, variablename_id);

	// Get the OpenGL handle:
	std::string variablename_string = GetStringForShaderVariableVariableNameId(variablename_id);
	switch (qualifier) {
	case SHADER_VARIABLE_QUALIFIER_attribute:
		newShaderHandle->glHandle = glGetAttribLocation(this->shaderProgram, variablename_string.c_str()); checkGlError("glGetAttribLocation"); // GLCALL
		FRAMEWORK->GetLogger()->dbglog("\nGLCALL glGetAttribLocation(\"%s\") = %u\n", variablename_string.c_str(), newShaderHandle->glHandle);
		break;

	case SHADER_VARIABLE_QUALIFIER_uniform:
		newShaderHandle->glHandle = glGetUniformLocation(this->shaderProgram, variablename_string.c_str()); checkGlError("glGetUniformLocation");	// GLCALL
		FRAMEWORK->GetLogger()->dbglog("\nGLCALL glGetUniformLocation(\"%s\") = %u\n", variablename_string.c_str(), newShaderHandle->glHandle);
		break;

	case SHADER_VARIABLE_QUALIFIER_varying:
		// Nothing to do for varyings
		break;

	default:
		ASSERT(0, "Unknown shader variable qualifier, %s", GetStringForShaderVariableQualifier(qualifier).c_str());
	}

	if (this->shaderHandles.find(variablename_id) != this->shaderHandles.end()) {
		// Duplicate shader variable handle
		VERIFY(qualifier == SHADER_VARIABLE_QUALIFIER_varying, "Duplicate shader variable handle has to be a varying");

	} else {
		this->shaderHandles[variablename_id] = newShaderHandle;
		FRAMEWORK->GetLogger()->dbglog("\nAdded shader variable handle for: %s", variablename_string.c_str());
	}
}

void ShaderHandles::destroy() {
	// TODO [Implement] Figure out how to cleanup shader variable handles here
}

