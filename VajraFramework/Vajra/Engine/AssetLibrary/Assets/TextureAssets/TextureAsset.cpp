#include "Vajra/Engine/AssetLibrary/Assets/TextureAssets/TextureAsset.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Loaders/TextureLoader/TextureLoader.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/Utilities.h"

AssetType TextureAsset::assetType = ASSET_TYPE_GL_TEXTURE;

TextureAsset::TextureAsset() : Asset() {
	this->init();
}

TextureAsset::TextureAsset(std::string urlOfTexture) : Asset(urlOfTexture) {
	this->init();
}

TextureAsset::~TextureAsset() {
}

void TextureAsset::init() {
	this->textureBytes = nullptr;
	this->textureGLHandle = 0;
}

void TextureAsset::destroy() {
	if (this->textureBytes != nullptr) {
		free(this->textureBytes);
	}
	if (this->textureGLHandle != 0) {
		glDeleteTextures(1, &(this->textureGLHandle));
	}
	// TODO [Implement] Figure out if any other cleanup is necessary to free up allocated opengl texture memory
}

void TextureAsset::Draw(GLint drawAsTextureUnit) {

	ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

	switch (drawAsTextureUnit) {
	case 0: {
		if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_myTextureSampler)) {
			GLint textureHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_myTextureSampler);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->textureGLHandle);
			checkGlError("glBindTexture");
			glUniform1i(textureHandle, drawAsTextureUnit);
		}
	} break;

	case 1: {
		if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_bakedAmbientGridTextureSampler)) {
			GLint textureHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_bakedAmbientGridTextureSampler);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, this->textureGLHandle);
			checkGlError("glBindTexture");
			glUniform1i(textureHandle, drawAsTextureUnit);
		}
	} break;

	case 2: {
		ASSERT(0, "Whoa. This is where the depth texture plugs in for shadows");
	} break;

	case 3: {
		if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_secondaryTexture)) {
			GLint textureHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_secondaryTexture);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, this->textureGLHandle);
			checkGlError("glBindTexture");
			glUniform1i(textureHandle, drawAsTextureUnit);
		}
	} break;

	default: {
		FRAMEWORK->GetLogger()->dbglog("\nValid texture unit %d", drawAsTextureUnit);
	} break;

	}

}

AssetType TextureAsset::GetAssetType() {
	return TextureAsset::assetType;
}

void TextureAsset::LoadAsset() {
	ASSERT(this->GetUrl() != "", "Url set when trying to load texture asset");

	FRAMEWORK->GetLogger()->dbglog("\nLoading texture asset from url: %s", this->GetFilePathToTexture().c_str());

	// TODO [Implement] Move loadGLTextureFromPNG into a Framework class/namespace
    // Load image
    this->textureGLHandle = loadGLTextureFromPNG(this->GetFilePathToTexture().c_str(), &(this->textureBytes));
    ASSERT(this->textureGLHandle != 0 && this->textureBytes != nullptr, "Successfully loaded texture from url %s", this->GetFilePathToTexture().c_str());
}

std::string TextureAsset::GetFilePathToTexture() {
	return this->GetUrl();
}
