#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include <Vajra/Engine/AssetLibrary/Assets/MeshAssets/Material/Material.h>
#include "Vajra/Engine/Core/Engine.h"

Material::Material() {
	this->init();
}

Material::~Material() {
	this->destroy();
}

void Material::WriteMaterialToShader(ShaderSet* currentShaderSet) {
	// TODO [Implement] WriteMaterialToShader
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
