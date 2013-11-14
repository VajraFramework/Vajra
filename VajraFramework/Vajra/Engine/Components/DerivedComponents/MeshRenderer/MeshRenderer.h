#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/AssetLibrary/Assets/MeshAssets/MeshAsset.h"

#include <memory>
#include <string>

// Forward Declarations:
class Object;
class Mesh;

class MeshRenderer : public Component {
public:
	MeshRenderer();
	MeshRenderer(Object* object_);
	virtual ~MeshRenderer();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	void InitMesh(std::string urlOfMesh);

	// @Override
	virtual void HandleMessage(Message* message);

	void Draw();

private:
	void init();
	void destroy();

	static unsigned int componentTypeId;

	std::shared_ptr<MeshAsset> meshAsset;
};

#endif // MESH_RENDERER_H
