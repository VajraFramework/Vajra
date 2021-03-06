#ifndef RENDER_BATCH_H
#define RENDER_BATCH_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/AssetLibrary/Assets/TextureAssets/TextureAsset.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/Renderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SimpleMesh.h"
#include "Vajra/Utilities/OpenGLIncludes.h"

#include <memory>
#include <string>

// Forward Declarations:
class GameObject;
class MeshAsset;
class Object;

// Do not expose this as a Component that can be added via xml
class RenderBatch_static : public Renderer, public SimpleMesh {
public:
	RenderBatch_static();
	RenderBatch_static(Object* object_);
	virtual ~RenderBatch_static();

	static inline ComponentIdType GetTypeId() { return Renderer::GetTypeId(); }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	// @Override
	virtual void Draw();

	void CreateRenderBatchFromMeshes(std::vector<ObjectIdType> staticObjectIds);

private:
	void init();
	void destroy();

	// Utility Functions:
	void makeVBOs();

	std::string shaderName;

	GameObject* gameobjectRef;

};

#endif // RENDER_BATCH_H
