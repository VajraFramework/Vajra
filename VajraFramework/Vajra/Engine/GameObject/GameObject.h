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
class SceneGraph;
class Transform;


class GameObject : public Object {
public:
	GameObject(SceneGraph* sceneGraph);
	virtual ~GameObject();

	inline Transform* GetTransform() { return this->transform; }
	std::string GetShaderName();

	virtual void Draw();

	bool IsVisible() { return this->visible; }
	void SetVisible(bool visible) { this->visible = visible; }

	SceneGraph* GetParentSceneGraph() { return this->parentSceneGraph; }

private:
	void init(SceneGraph* sceneGraph);
	void destroy();

	Transform* transform;
	bool visible;

	SceneGraph* parentSceneGraph;
};

#endif // GAMEOBJECT_H
