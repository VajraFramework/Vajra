#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Lighting/AmbientLighting.h"

AmbientLighting::AmbientLighting() {
	// Don't call init here
}

AmbientLighting::~AmbientLighting() {
	this->destroy();
}

void AmbientLighting::SetPathToBakedAmbientLightTexture(const char* filePath) {
	this->bakedAmbientLightTextureAsset = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(filePath);
}

void AmbientLighting::Draw() {
	if (this->bakedAmbientLightTextureAsset != nullptr) {
		this->bakedAmbientLightTextureAsset->Draw(1);
	}
}

void AmbientLighting::init() {
	// Initialize with a black texture:
	this->bakedAmbientLightTextureAsset = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "black.png");
}

void AmbientLighting::destroy() {
}
