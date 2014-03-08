#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/AssetLibrary/Assets/TextureAssets/TextureAsset.h"

#include <memory>
#include <string>

// Forward Declarations:
class Camera;

class ShadowMap : public Object {
public:
	~ShadowMap();

	void Draw();

	void SetDepthCamera(Camera* camera) { this->depthCamera = camera; }

	// TODO [Hack] Make this private:
    GLuint depthTexture;

private:
	ShadowMap();
	void init();
	void destroy();

	Camera* depthCamera;


	friend class Engine;
};


#endif // SHADOW_MAP_H
