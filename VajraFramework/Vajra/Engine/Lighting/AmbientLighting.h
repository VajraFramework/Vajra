#ifndef AMBIENT_LIGHTING_H
#define AMBIENT_LIGHTING_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/AssetLibrary/Assets/TextureAssets/TextureAsset.h"

#include <memory>
#include <string>

class AmbientLighting : public Object {
public:
	~AmbientLighting();

	void SetPathToBakedAmbientLightTexture(const char* filePath);
	void Draw();

private:
	AmbientLighting();
	void init();
	void destroy();

	std::shared_ptr<TextureAsset> bakedAmbientLightTextureAsset;

	friend class Engine;
};

#endif // AMBIENT_LIGHTING_H
