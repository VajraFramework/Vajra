#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Common/Objects/Declarations.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

#include <list>
#include <map>
#include <string>
#include <typeinfo>

// Forward Declarations:
class Component;
class Mesh;
class SceneGraph;
class Transform;


class GameObject : public Object {
public:
	GameObject();
	virtual ~GameObject();

	inline Transform* GetTransform() { return this->transform; }
	inline Mesh* GetMesh() { return this->model; }

	virtual void Update();
	virtual void Draw();


private:
	void init();
	void destroy();

	Transform* transform;
	Mesh* model;
};

#endif // GAMEOBJECT_H
