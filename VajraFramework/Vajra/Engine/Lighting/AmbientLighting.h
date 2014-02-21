#ifndef AMBIENT_LIGHTING_H
#define AMBIENT_LIGHTING_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/AssetLibrary/Assets/TextureAssets/TextureAsset.h"

#include <memory>
#include <string>

class AmbientLighting : public Object {
public:
	~AmbientLighting();

	void SetBakedAmbientLightTexture(const char* filePath, float widthInWorld_, float heightInWorld_);
	void Draw();

private:
	AmbientLighting();
	void init();
	void destroy();

	float widthInWorld;
	float heightInWorld;
	std::shared_ptr<TextureAsset> bakedAmbientLightTextureAsset;

	friend class Engine;
};

#endif // AMBIENT_LIGHTING_H
