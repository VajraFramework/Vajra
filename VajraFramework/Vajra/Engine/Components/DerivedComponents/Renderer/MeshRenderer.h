#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/AssetLibrary/Assets/MeshAssets/MeshAsset.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/Renderer.h"

#include <memory>
#include <string>

// Forward Declarations:
class Object;
class Mesh;

class MeshRenderer : public Renderer {
public:
	MeshRenderer();
	MeshRenderer(Object* object_);
	virtual ~MeshRenderer();

	static inline ComponentIdType GetTypeId() { return Renderer::GetTypeId(); }

	void InitMesh(std::string urlOfMesh);

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	// @Override
	virtual void Draw();

private:
	void init();
	void destroy();

	std::shared_ptr<MeshAsset> meshAsset;
};

#endif // MESH_RENDERER_H
