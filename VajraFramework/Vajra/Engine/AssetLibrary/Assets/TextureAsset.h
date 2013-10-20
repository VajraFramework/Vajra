#ifndef TEXTURE_ASSET_H
#define TEXTURE_ASSET_H

#include "Vajra/Engine/AssetLibrary/Asset.h"

#ifdef PLATFORM_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif // PLATFORM_ANDROID

#ifdef PLATFORM_DESKTOP
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GL/glfw.h>
#endif // PLATFORM_DESKTOP

#ifdef PLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif // PLATFORM_IOS

class TextureAsset : public Asset {
public:
	TextureAsset();
	TextureAsset(std::string urlOfTexture);
	~TextureAsset();

	std::string GetFilePathToTexture();
	inline GLint GetGLTextureHandle() { return this->textureGLHandle; }

	// @Override
	virtual AssetType GetAssetType();
	// @Override
	virtual void LoadAsset();

private:

	static AssetType assetType;

	GLint textureGLHandle;
};

#endif // TEXTURE_ASSET_H
