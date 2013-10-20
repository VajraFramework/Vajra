#ifndef TEXTURE_ASSET_H
#define TEXTURE_ASSET_H

#include "Vajra/Engine/AssetLibrary/Asset.h"
#include "Vajra/Utilities/OpenGLIncludes.h"

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
	void init();
	void destroy();

	static AssetType assetType;

	GLubyte* textureBytes;
	GLint textureGLHandle;
};

#endif // TEXTURE_ASSET_H
