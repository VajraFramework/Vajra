#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Lighting/AmbientLighting.h"
#include "Vajra/Utilities/Utilities.h"

AmbientLighting::AmbientLighting() {
	// Don't call init here
}

AmbientLighting::~AmbientLighting() {
	this->destroy();
}

void AmbientLighting::SetBakedAmbientLightTexture(const char* filePath, float widthInWorld_, float heightInWorld_) {
	VERIFY(widthInWorld_ != 0 && heightInWorld_ != 0, "Width and height not 0 (%d, %d)", widthInWorld_, heightInWorld_);
	this->widthInWorld  = widthInWorld_;
	this->heightInWorld = heightInWorld_;
	this->bakedAmbientLightTextureAsset = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(filePath);
}

void AmbientLighting::Draw() {
	if (this->bakedAmbientLightTextureAsset != nullptr) {
		this->bakedAmbientLightTextureAsset->Draw(1);
	}
}

void AmbientLighting::init() {
	// Initialize with a black texture:
	this->bakedAmbientLightTextureAsset = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(
										  FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "black.png");
	// Some sensible numbers:
	this->widthInWorld  = 100.0f;
	this->heightInWorld = 100.0f;
}

void AmbientLighting::destroy() {
}
