#include "Vajra/Framework/OpenGL/ShaderHandles/Declarations.h"

Shader_variable_qualifier_t GetShaderVariableQualifierFromString(std::string s) {

	if (s == "attribute") {
		return SHADER_VARIABLE_QUALIFIER_attribute;
	}
	if (s == "uniform") {
		return SHADER_VARIABLE_QUALIFIER_uniform;
	}
	if (s == "varying") {
		return SHADER_VARIABLE_QUALIFIER_varying;
	}
	if (s == "invalid") {
		return SHADER_VARIABLE_QUALIFIER_invalid;
	}
	return SHADER_VARIABLE_QUALIFIER_invalid;
}

Shader_variable_datatype_t GetShaderVariableDatatypeFromString(std::string s) {

	if (s == "vec2") {
		return SHADER_VARIABLE_DATATYPE_vec2;
	}
	if (s == "mat4") {
		return SHADER_VARIABLE_DATATYPE_mat4;
	}
	if (s == "vec4") {
		return SHADER_VARIABLE_DATATYPE_vec4;
	}
	if (s == "invalid") {
		return SHADER_VARIABLE_DATATYPE_invalid;
	}
	return SHADER_VARIABLE_DATATYPE_invalid;
}

Shader_variable_variablename_id_t GetShaderVariableVariableNameIdFromString(std::string s) {

	if (s == "vPosition") {
		return SHADER_VARIABLE_VARIABLENAME_vPosition;
	}
	if (s == "vNormal") {
		return SHADER_VARIABLE_VARIABLENAME_vNormal;
	}
	if (s == "uvCoords_in") {
		return SHADER_VARIABLE_VARIABLENAME_uvCoords_in;
	}
	if (s == "mvpMatrix") {
		return SHADER_VARIABLE_VARIABLENAME_mvpMatrix;
	}
	if (s == "modelInverseTransposeMatrix") {
		return SHADER_VARIABLE_VARIABLENAME_modelInverseTransposeMatrix;
	}
	if (s == "uvCoords_out") {
		return SHADER_VARIABLE_VARIABLENAME_uvCoords_out;
	}
	if (s == "lightIntensity") {
		return SHADER_VARIABLE_VARIABLENAME_lightIntensity;
	}
	if (s == "DLight0Direction") {
		return SHADER_VARIABLE_VARIABLENAME_DLight0Direction;
	}
	if (s == "DLight0AmbientColor") {
		return SHADER_VARIABLE_VARIABLENAME_DLight0AmbientColor;
	}
	if (s == "DLight0DiffuseColor") {
		return SHADER_VARIABLE_VARIABLENAME_DLight0DiffuseColor;
	}
	if (s == "DLight0SpecularColor") {
		return SHADER_VARIABLE_VARIABLENAME_DLight0SpecularColor;
	}
	if (s == "invalid") {
		return SHADER_VARIABLE_VARIABLENAME_invalid;
	}
	return SHADER_VARIABLE_VARIABLENAME_invalid;
}

std::string GetStringForShaderVariableQualifier(Shader_variable_qualifier_t t) {
	switch (t) {

	case SHADER_VARIABLE_QUALIFIER_attribute: return "attribute";
	case SHADER_VARIABLE_QUALIFIER_uniform: return "uniform";
	case SHADER_VARIABLE_QUALIFIER_varying: return "varying";
	case SHADER_VARIABLE_QUALIFIER_invalid: return "invalid";
	// default: DO NOT ADD A DEFAULT HERE. It'll help catch the case where the enum is extended but not handled here
	}
	return "invalid";
}

std::string GetStringForShaderVariableDatatype(Shader_variable_datatype_t t) {
	switch (t) {

	case SHADER_VARIABLE_DATATYPE_vec2: return "vec2";
	case SHADER_VARIABLE_DATATYPE_mat4: return "mat4";
	case SHADER_VARIABLE_DATATYPE_vec4: return "vec4";
	case SHADER_VARIABLE_DATATYPE_invalid: return "invalid";
	// default: DO NOT ADD A DEFAULT HERE. It'll help catch the case where the enum is extended but not handled here
	}
	return "invalid";
}

std::string GetStringForShaderVariableVariableNameId(Shader_variable_variablename_id_t t) {
	switch (t) {

	case SHADER_VARIABLE_VARIABLENAME_vPosition: return "vPosition";
	case SHADER_VARIABLE_VARIABLENAME_vNormal: return "vNormal";
	case SHADER_VARIABLE_VARIABLENAME_uvCoords_in: return "uvCoords_in";
	case SHADER_VARIABLE_VARIABLENAME_mvpMatrix: return "mvpMatrix";
	case SHADER_VARIABLE_VARIABLENAME_modelInverseTransposeMatrix: return "modelInverseTransposeMatrix";
	case SHADER_VARIABLE_VARIABLENAME_uvCoords_out: return "uvCoords_out";
	case SHADER_VARIABLE_VARIABLENAME_lightIntensity: return "lightIntensity";
	case SHADER_VARIABLE_VARIABLENAME_DLight0Direction: return "DLight0Direction";
	case SHADER_VARIABLE_VARIABLENAME_DLight0AmbientColor: return "DLight0AmbientColor";
	case SHADER_VARIABLE_VARIABLENAME_DLight0DiffuseColor: return "DLight0DiffuseColor";
	case SHADER_VARIABLE_VARIABLENAME_DLight0SpecularColor: return "DLight0SpecularColor";
	case SHADER_VARIABLE_VARIABLENAME_invalid: return "invalid";
	// default: DO NOT ADD A DEFAULT HERE. It'll help catch the case where the enum is extended but not handled here
	}
	return "invalid";
}

