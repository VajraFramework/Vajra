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

	Camera* GetDepthCamera();

	void SetOrthoBounds(float left, float right, float bottom, float top, float near, float far);


	// TODO [Hack] Make this private:
    GLuint depthTexture;

private:
	ShadowMap();
	void init();
	void destroy();

	ObjectIdType depthCameraId;

	void createDepthCamera();
	void adjustDepthCamera();

	void mainCameraChanged();
	void mainDirectionalLightChanged();

	float ortho_bounds_left;
	float ortho_bounds_right;
	float ortho_bounds_bottom;
	float ortho_bounds_top;
	float ortho_bounds_near;
	float ortho_bounds_far;

	friend class Engine;
	friend class ShadowMapUpdateListener;
};


#endif // SHADOW_MAP_H
