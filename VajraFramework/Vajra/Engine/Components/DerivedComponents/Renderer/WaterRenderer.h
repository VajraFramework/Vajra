#ifndef WATER_RENDERER_H
#define WATER_RENDERER_H

#include "Vajra/Engine/Components/DerivedComponents/Renderer/MeshRenderer.h"

// Forward Declarations:
class Object;
class Mesh;

//[[COMPONENT]]//
class WaterRenderer : public MeshRenderer {
public:
	WaterRenderer();
	WaterRenderer(Object* object_);
	virtual ~WaterRenderer();

	static inline ComponentIdType GetTypeId() { return Renderer::GetTypeId(); }

	//[[PROPERTY]]//
	void InitMesh(std::string urlOfMesh);

	//[[PROPERTY]]//
	void SetScrollingUVs(float uvScrollSpeed);

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	// @Override
	virtual void Draw();

private:
	void init();
	void destroy();

	// Utility Functions:
	void scrollUVs();

	float scrollingUVsOffset;

	float scrollingUVs_speed;
};

#endif // WATER_RENDERER_H
