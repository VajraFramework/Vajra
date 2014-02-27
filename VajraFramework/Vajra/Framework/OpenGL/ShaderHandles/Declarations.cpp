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

	if (s == "float") {
		return SHADER_VARIABLE_DATATYPE_float;
	}
	if (s == "sampler2D") {
		return SHADER_VARIABLE_DATATYPE_sampler2D;
	}
	if (s == "vec2") {
		return SHADER_VARIABLE_DATATYPE_vec2;
	}
	if (s == "mat4") {
		return SHADER_VARIABLE_DATATYPE_mat4;
	}
	if (s == "vec3") {
		return SHADER_VARIABLE_DATATYPE_vec3;
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
	if (s == "boneIndices") {
		return SHADER_VARIABLE_VARIABLENAME_boneIndices;
	}
	if (s == "boneWeights") {
		return SHADER_VARIABLE_VARIABLENAME_boneWeights;
	}
	if (s == "mvpMatrix") {
		return SHADER_VARIABLE_VARIABLENAME_mvpMatrix;
	}
	if (s == "modelMatrix") {
		return SHADER_VARIABLE_VARIABLENAME_modelMatrix;
	}
	if (s == "vpMatrix") {
		return SHADER_VARIABLE_VARIABLENAME_vpMatrix;
	}
	if (s == "modelInverseTransposeMatrix") {
		return SHADER_VARIABLE_VARIABLENAME_modelInverseTransposeMatrix;
	}
	if (s == "vNormal_out") {
		return SHADER_VARIABLE_VARIABLENAME_vNormal_out;
	}
	if (s == "uvCoords_out") {
		return SHADER_VARIABLE_VARIABLENAME_uvCoords_out;
	}
	if (s == "lightIntensity") {
		return SHADER_VARIABLE_VARIABLENAME_lightIntensity;
	}
	if (s == "boneTransforms") {
		return SHADER_VARIABLE_VARIABLENAME_boneTransforms;
	}
	if (s == "otherBoneTransforms") {
		return SHADER_VARIABLE_VARIABLENAME_otherBoneTransforms;
	}
	if (s == "interp") {
		return SHADER_VARIABLE_VARIABLENAME_interp;
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
	if (s == "DLight0Direction_out") {
		return SHADER_VARIABLE_VARIABLENAME_DLight0Direction_out;
	}
	if (s == "DLight0AmbientColor_out") {
		return SHADER_VARIABLE_VARIABLENAME_DLight0AmbientColor_out;
	}
	if (s == "DLight0DiffuseColor_out") {
		return SHADER_VARIABLE_VARIABLENAME_DLight0DiffuseColor_out;
	}
	if (s == "DLight0SpecularColor_out") {
		return SHADER_VARIABLE_VARIABLENAME_DLight0SpecularColor_out;
	}
	if (s == "MaterialAmbientColor") {
		return SHADER_VARIABLE_VARIABLENAME_MaterialAmbientColor;
	}
	if (s == "MaterialDiffuseColor") {
		return SHADER_VARIABLE_VARIABLENAME_MaterialDiffuseColor;
	}
	if (s == "MaterialSpecularColor") {
		return SHADER_VARIABLE_VARIABLENAME_MaterialSpecularColor;
	}
	if (s == "MaterialAmbientColor_out") {
		return SHADER_VARIABLE_VARIABLENAME_MaterialAmbientColor_out;
	}
	if (s == "MaterialDiffuseColor_out") {
		return SHADER_VARIABLE_VARIABLENAME_MaterialDiffuseColor_out;
	}
	if (s == "MaterialSpecularColor_out") {
		return SHADER_VARIABLE_VARIABLENAME_MaterialSpecularColor_out;
	}
	if (s == "pointSize") {
		return SHADER_VARIABLE_VARIABLENAME_pointSize;
	}
	if (s == "particleColor") {
		return SHADER_VARIABLE_VARIABLENAME_particleColor;
	}
	if (s == "myTextureSampler") {
		return SHADER_VARIABLE_VARIABLENAME_myTextureSampler;
	}
	if (s == "bakedAmbientGridTextureSampler") {
		return SHADER_VARIABLE_VARIABLENAME_bakedAmbientGridTextureSampler;
	}
	if (s == "modelCoordsForBakedAmbientLookup_out") {
		return SHADER_VARIABLE_VARIABLENAME_modelCoordsForBakedAmbientLookup_out;
	}
	if (s == "bakedLightMapSize") {
		return SHADER_VARIABLE_VARIABLENAME_bakedLightMapSize;
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

	case SHADER_VARIABLE_DATATYPE_float: return "float";
	case SHADER_VARIABLE_DATATYPE_sampler2D: return "sampler2D";
	case SHADER_VARIABLE_DATATYPE_vec2: return "vec2";
	case SHADER_VARIABLE_DATATYPE_mat4: return "mat4";
	case SHADER_VARIABLE_DATATYPE_vec3: return "vec3";
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
	case SHADER_VARIABLE_VARIABLENAME_boneIndices: return "boneIndices";
	case SHADER_VARIABLE_VARIABLENAME_boneWeights: return "boneWeights";
	case SHADER_VARIABLE_VARIABLENAME_mvpMatrix: return "mvpMatrix";
	case SHADER_VARIABLE_VARIABLENAME_modelMatrix: return "modelMatrix";
	case SHADER_VARIABLE_VARIABLENAME_vpMatrix: return "vpMatrix";
	case SHADER_VARIABLE_VARIABLENAME_modelInverseTransposeMatrix: return "modelInverseTransposeMatrix";
	case SHADER_VARIABLE_VARIABLENAME_vNormal_out: return "vNormal_out";
	case SHADER_VARIABLE_VARIABLENAME_uvCoords_out: return "uvCoords_out";
	case SHADER_VARIABLE_VARIABLENAME_lightIntensity: return "lightIntensity";
	case SHADER_VARIABLE_VARIABLENAME_boneTransforms: return "boneTransforms";
	case SHADER_VARIABLE_VARIABLENAME_otherBoneTransforms: return "otherBoneTransforms";
	case SHADER_VARIABLE_VARIABLENAME_interp: return "interp";
	case SHADER_VARIABLE_VARIABLENAME_DLight0Direction: return "DLight0Direction";
	case SHADER_VARIABLE_VARIABLENAME_DLight0AmbientColor: return "DLight0AmbientColor";
	case SHADER_VARIABLE_VARIABLENAME_DLight0DiffuseColor: return "DLight0DiffuseColor";
	case SHADER_VARIABLE_VARIABLENAME_DLight0SpecularColor: return "DLight0SpecularColor";
	case SHADER_VARIABLE_VARIABLENAME_DLight0Direction_out: return "DLight0Direction_out";
	case SHADER_VARIABLE_VARIABLENAME_DLight0AmbientColor_out: return "DLight0AmbientColor_out";
	case SHADER_VARIABLE_VARIABLENAME_DLight0DiffuseColor_out: return "DLight0DiffuseColor_out";
	case SHADER_VARIABLE_VARIABLENAME_DLight0SpecularColor_out: return "DLight0SpecularColor_out";
	case SHADER_VARIABLE_VARIABLENAME_MaterialAmbientColor: return "MaterialAmbientColor";
	case SHADER_VARIABLE_VARIABLENAME_MaterialDiffuseColor: return "MaterialDiffuseColor";
	case SHADER_VARIABLE_VARIABLENAME_MaterialSpecularColor: return "MaterialSpecularColor";
	case SHADER_VARIABLE_VARIABLENAME_MaterialAmbientColor_out: return "MaterialAmbientColor_out";
	case SHADER_VARIABLE_VARIABLENAME_MaterialDiffuseColor_out: return "MaterialDiffuseColor_out";
	case SHADER_VARIABLE_VARIABLENAME_MaterialSpecularColor_out: return "MaterialSpecularColor_out";
	case SHADER_VARIABLE_VARIABLENAME_pointSize: return "pointSize";
	case SHADER_VARIABLE_VARIABLENAME_particleColor: return "particleColor";
	case SHADER_VARIABLE_VARIABLENAME_myTextureSampler: return "myTextureSampler";
	case SHADER_VARIABLE_VARIABLENAME_bakedAmbientGridTextureSampler: return "bakedAmbientGridTextureSampler";
	case SHADER_VARIABLE_VARIABLENAME_modelCoordsForBakedAmbientLookup_out: return "modelCoordsForBakedAmbientLookup_out";
	case SHADER_VARIABLE_VARIABLENAME_bakedLightMapSize: return "bakedLightMapSize";
	case SHADER_VARIABLE_VARIABLENAME_invalid: return "invalid";
	// default: DO NOT ADD A DEFAULT HERE. It'll help catch the case where the enum is extended but not handled here
	}
	return "invalid";
}

