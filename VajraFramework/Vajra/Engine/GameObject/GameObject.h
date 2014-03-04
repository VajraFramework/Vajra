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

	virtual void AddChild(ObjectIdType childId);
	virtual void SetParent(ObjectIdType newParentId);
	// Adds the child game object to this, and makes sure that the child remains at the "same" transform in the world:
	void AddChild_maintainTransform(ObjectIdType childId);

	inline Transform* GetTransform() { return this->transform; }
	std::string GetShaderName();

	virtual void Draw();

	bool IsVisible() { return this->visible; }
	virtual void SetVisible(bool visible_);

	bool HasTransperancy();
	bool IsOverlay();

	SceneGraph* GetParentSceneGraph() { return this->parentSceneGraph; }

	void AddTag(std::string tag);
	void AddTag(unsigned int tagBit);

	void RemoveTag(std::string tag);
	void RemoveTag(unsigned int tagBit);

	bool HasTag(std::string tag);
	bool HasTag(unsigned int tagBit);

private:
	void init(SceneGraph* sceneGraph);
	void destroy();

	Transform* transform;
	bool visible;

	unsigned int tags; // bitmask
	SceneGraph* parentSceneGraph;
};

#endif // GAMEOBJECT_H
