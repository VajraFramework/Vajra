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

//[[COMPONENT]]//
class MeshRenderer : public Renderer {
public:
	MeshRenderer();
	MeshRenderer(Object* object_);
	virtual ~MeshRenderer();

	static inline ComponentIdType GetTypeId() { return Renderer::GetTypeId(); }

	//[[PROPERTY]]//
	void InitMesh(std::string urlOfMesh);

	//[[PROPERTY]]//
	inline void SetIsStatic(bool isStatic_);

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	// @Override
	virtual void Draw();

	MeshAsset* GetMeshAsset() { return this->meshAsset.get(); }

private:
	void init();
	void destroy();

	std::shared_ptr<MeshAsset> meshAsset;
};

////////////////////////////////////////////////////////////////////////////////
// Inline Functions:

inline void MeshRenderer::SetIsStatic(bool isStatic_) {
	Renderer::SetIsStatic(isStatic_);
}

#endif // MESH_RENDERER_H
