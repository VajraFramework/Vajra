#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Vajra/Common/Components/Component.h"

// Forward Declarations:
class Object;
class Mesh;

class MeshRenderer : public Component {
public:
	MeshRenderer();
	MeshRenderer(Object* object_);
	virtual ~MeshRenderer();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	void InitMesh(Mesh* newMesh);

	// @Override
	virtual void HandleMessage(Message* message);

	void Draw();

private:
	void init();
	void destroy();

	static unsigned int componentTypeId;

	Mesh* mesh;
};

#endif // MESH_RENDERER_H
