#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AssetLibrary/Assets/MeshAssets/Material/Material.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

Material::Material() {
	this->init();
}

Material::~Material() {
	this->destroy();
}

void Material::WriteMaterialToShader() {
	if (FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->HasHandle(SHADER_VARIABLE_VARIABLENAME_MaterialAmbientColor)) {
		glUniform4f(FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->GetHandle(SHADER_VARIABLE_VARIABLENAME_MaterialAmbientColor),
					this->ambientColor.r, this->ambientColor.g, this->ambientColor.b, this->ambientColor.a);
		glUniform4f(FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->GetHandle(SHADER_VARIABLE_VARIABLENAME_MaterialDiffuseColor),
					this->diffuseColor.r, this->diffuseColor.g, this->diffuseColor.b, this->diffuseColor.a);
		glUniform4f(FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->GetHandle(SHADER_VARIABLE_VARIABLENAME_MaterialSpecularColor),
					this->specularColor.r, this->specularColor.g, this->specularColor.b, this->specularColor.a);
	}
}

void Material::init() {
	this->ambientColor  = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	this->diffuseColor  = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	this->specularColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
}

void Material::SetTextureFilePath(std::string filePath) {
	this->textureAsset = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(filePath);
}

void Material::destroy() {
}
