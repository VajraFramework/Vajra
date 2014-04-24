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
	this->destroy();
}

void TextureAsset::init() {
	this->textureGLHandle = 0;
}

void TextureAsset::destroy() {
	if (this->textureGLHandle != 0) {
		FRAMEWORK->GetOpenGLWrapper()->FreeGLTexture(&this->textureGLHandle);
	}
	// TODO [Implement] Figure out if any other cleanup is necessary to free up allocated opengl texture memory
}

void TextureAsset::Draw(GLint drawAsTextureUnit) {

	ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

	switch (drawAsTextureUnit) {
	case 0: {
		if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_myTextureSampler)) {
			GLint textureHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_myTextureSampler);

			GLCALL(glActiveTexture, GL_TEXTURE0);
			GLCALL(glBindTexture, GL_TEXTURE_2D, this->textureGLHandle);
			GLCALL(glUniform1i, textureHandle, drawAsTextureUnit);
		}
	} break;

	case 1: {
	} break;

	case 2: {
		ASSERT(0, "Whoa. This is where the depth texture plugs in for shadows");
	} break;

	case 3: {
		if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_secondaryTexture)) {
			GLint textureHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_secondaryTexture);

			GLCALL(glActiveTexture, GL_TEXTURE3);
			GLCALL(glBindTexture, GL_TEXTURE_2D, this->textureGLHandle);
			GLCALL(glUniform1i, textureHandle, drawAsTextureUnit);

		} else if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_normalMap)) {
			GLint textureHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_normalMap);

			GLCALL(glActiveTexture, GL_TEXTURE3);
			GLCALL(glBindTexture, GL_TEXTURE_2D, this->textureGLHandle);
			GLCALL(glUniform1i, textureHandle, drawAsTextureUnit);
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
	GLubyte* textureBytes = nullptr;
	bool useMipmaps = textureMustUseMipmaps(this->GetFilePathToTexture());
    this->textureGLHandle = loadGLTextureFromPNG(this->GetFilePathToTexture().c_str(), &(textureBytes), useMipmaps);
    ASSERT(this->textureGLHandle != 0 && textureBytes != nullptr, "Successfully loaded texture from url %s", this->GetFilePathToTexture().c_str());

    // Free up the texture bytes in ram now that it's in the gpu:
	if (textureBytes != nullptr) {
		free(textureBytes);
	}
	textureBytes = nullptr;
}

std::string TextureAsset::GetFilePathToTexture() {
	return this->GetUrl();
}
