#ifndef SHADER_SET_CREATION_HELPER_H
#define SHADER_SET_CREATION_HELPER_H

#include <string>

namespace ShaderSetCreationHelper {
	std::string GetVariableDeclarationForVariableName(std::string variableName);
	//
	std::string GetVariableQualifierForVariableName(std::string variableName);
	std::string GetVariableDatatypeForVariableName (std::string variableName);

	void LoadPreprocessorVariables();
	bool IsPreprocessorVariableDefined(std::string variableName);

	std::string CleanupShaderSourceForPreprocessorDirectives(std::string shaderSource);
}

#endif // SHADER_SET_CREATION_HELPER_H
