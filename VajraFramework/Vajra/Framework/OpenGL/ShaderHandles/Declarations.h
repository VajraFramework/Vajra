#ifndef SHADER_HANDLES_DECLARATIONS_H
#define SHADER_HANDLES_DECLARATIONS_H

#include <string>

/*
 * NOTE: When any of shader variables in shader_variables are modified, this file has to be regenerated by running generateEnums.pl in Tools/ShaderTools/
 */


enum Shader_variable_qualifier_t {
	SHADER_VARIABLE_QUALIFIER_attribute,
	SHADER_VARIABLE_QUALIFIER_uniform,
	SHADER_VARIABLE_QUALIFIER_varying,
	SHADER_VARIABLE_QUALIFIER_invalid,
};

enum Shader_variable_datatype_t {
	SHADER_VARIABLE_DATATYPE_sampler2DShadow,
	SHADER_VARIABLE_DATATYPE_float,
	SHADER_VARIABLE_DATATYPE_sampler2D,
	SHADER_VARIABLE_DATATYPE_vec2,
	SHADER_VARIABLE_DATATYPE_mat4,
	SHADER_VARIABLE_DATATYPE_vec3,
	SHADER_VARIABLE_DATATYPE_vec4,
	SHADER_VARIABLE_DATATYPE_invalid,
};

enum Shader_variable_variablename_id_t {
	SHADER_VARIABLE_VARIABLENAME_vPosition,
	SHADER_VARIABLE_VARIABLENAME_vNormal,
	SHADER_VARIABLE_VARIABLENAME_uvCoords_in,
	SHADER_VARIABLE_VARIABLENAME_boneIndices,
	SHADER_VARIABLE_VARIABLENAME_boneWeights,
	SHADER_VARIABLE_VARIABLENAME_mvpMatrix,
	SHADER_VARIABLE_VARIABLENAME_modelMatrix,
	SHADER_VARIABLE_VARIABLENAME_vpMatrix,
	SHADER_VARIABLE_VARIABLENAME_modelInverseTransposeMatrix,
	SHADER_VARIABLE_VARIABLENAME_vNormal_out,
	SHADER_VARIABLE_VARIABLENAME_uvCoords_out,
	SHADER_VARIABLE_VARIABLENAME_lightIntensity,
	SHADER_VARIABLE_VARIABLENAME_boneTransforms,
	SHADER_VARIABLE_VARIABLENAME_otherBoneTransforms,
	SHADER_VARIABLE_VARIABLENAME_interp,
	SHADER_VARIABLE_VARIABLENAME_DLight0Direction,
	SHADER_VARIABLE_VARIABLENAME_DLight0AmbientColor,
	SHADER_VARIABLE_VARIABLENAME_DLight0DiffuseColor,
	SHADER_VARIABLE_VARIABLENAME_DLight0SpecularColor,
	SHADER_VARIABLE_VARIABLENAME_DLight0Direction_out,
	SHADER_VARIABLE_VARIABLENAME_DLight0AmbientColor_out,
	SHADER_VARIABLE_VARIABLENAME_DLight0DiffuseColor_out,
	SHADER_VARIABLE_VARIABLENAME_DLight0SpecularColor_out,
	SHADER_VARIABLE_VARIABLENAME_MaterialAmbientColor,
	SHADER_VARIABLE_VARIABLENAME_MaterialDiffuseColor,
	SHADER_VARIABLE_VARIABLENAME_MaterialSpecularColor,
	SHADER_VARIABLE_VARIABLENAME_MaterialAmbientColor_out,
	SHADER_VARIABLE_VARIABLENAME_MaterialDiffuseColor_out,
	SHADER_VARIABLE_VARIABLENAME_MaterialSpecularColor_out,
	SHADER_VARIABLE_VARIABLENAME_AdditionalLight0Direction,
	SHADER_VARIABLE_VARIABLENAME_AdditionalLight0Ambient,
	SHADER_VARIABLE_VARIABLENAME_AdditionalLight0Diffuse,
	SHADER_VARIABLE_VARIABLENAME_AdditionalLight1Direction,
	SHADER_VARIABLE_VARIABLENAME_AdditionalLight1Ambient,
	SHADER_VARIABLE_VARIABLENAME_AdditionalLight1Diffuse,
	SHADER_VARIABLE_VARIABLENAME_AdditionalLight2Direction,
	SHADER_VARIABLE_VARIABLENAME_AdditionalLight2Ambient,
	SHADER_VARIABLE_VARIABLENAME_AdditionalLight2Diffuse,
	SHADER_VARIABLE_VARIABLENAME_pointSize,
	SHADER_VARIABLE_VARIABLENAME_particleColor,
	SHADER_VARIABLE_VARIABLENAME_myTextureSampler,
	SHADER_VARIABLE_VARIABLENAME_bakedAmbientGridTextureSampler,
	SHADER_VARIABLE_VARIABLENAME_modelCoordsForBakedAmbientLookup_out,
	SHADER_VARIABLE_VARIABLENAME_bakedLightMapSize,
	SHADER_VARIABLE_VARIABLENAME_depthTextureSampler,
	SHADER_VARIABLE_VARIABLENAME_depthBiasMVPMatrix,
	SHADER_VARIABLE_VARIABLENAME_ShadowCoord,
	SHADER_VARIABLE_VARIABLENAME_invalid,
};

Shader_variable_qualifier_t GetShaderVariableQualifierFromString(std::string s);
Shader_variable_datatype_t GetShaderVariableDatatypeFromString(std::string s);
Shader_variable_variablename_id_t GetShaderVariableVariableNameIdFromString(std::string s);

std::string GetStringForShaderVariableQualifier(Shader_variable_qualifier_t t);
std::string GetStringForShaderVariableDatatype(Shader_variable_datatype_t t);
std::string GetStringForShaderVariableVariableNameId(Shader_variable_variablename_id_t t);

#endif // SHADER_HANDLES_DECLARATIONS_H

