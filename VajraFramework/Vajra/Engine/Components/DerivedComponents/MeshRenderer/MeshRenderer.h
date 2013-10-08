#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Vajra/Engine/Components/BaseComponent/Component.h"

// Forward Declarations:
class GameObject;
class Mesh;

class MeshRenderer : public Component {
public:
	MeshRenderer();
	MeshRenderer(GameObject* gameObject_);
	virtual ~MeshRenderer();

	static inline unsigned int GetTypeId() { return componentTypeId; }

	void InitMesh(Mesh* newMesh);

	// @Override
	virtual void Update();

	void Draw();

private:
	void init();
	void destroy();

	static unsigned int componentTypeId;

	Mesh* mesh;
};

#endif // MESH_RENDERER_H