#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Lighting/AmbientLighting.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/MathUtilities.h"
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

void AmbientLighting::ResetBakedAmbientLightTexture() {
	// Initialize with a black texture:
	this->bakedAmbientLightTextureAsset = ENGINE->GetAssetLibrary()->GetAsset<TextureAsset>(
										  FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "black.png");
	// Some sensible numbers:
	this->widthInWorld  = 100.0f;
	this->heightInWorld = 100.0f;
}

void AmbientLighting::SetIntensity(IntensityLevel_t intensityLevel) {
	switch (intensityLevel) {
	case INTENSITY_low   : this->intensity = 0.05f; return;
	case INTENSITY_medium: this->intensity = 0.1f;  return;
	case INTENSITY_high  : this->intensity = 0.2f;  return;
	default              : this->intensity = 0.1f;  return;
	}
}

void AmbientLighting::Draw() {
	ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_bakedLightMapSize)) {
		GLint bakedLightMapSizeHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_bakedLightMapSize);
		glUniform1f(bakedLightMapSizeHandle, (float)std::max(this->widthInWorld, this->heightInWorld));
	}

	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_baked_ambient_intensity)) {
		GLint bakedLightMapSizeHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_baked_ambient_intensity);
		glUniform1f(bakedLightMapSizeHandle, this->intensity);
	}

	if (this->bakedAmbientLightTextureAsset != nullptr) {
		this->bakedAmbientLightTextureAsset->Draw(1);
	}
}

void AmbientLighting::init() {
	this->ResetBakedAmbientLightTexture();

	this->intensity = 0.2f;
}

void AmbientLighting::destroy() {
}

