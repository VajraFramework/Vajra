#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Input/ITouchTarget.h"
#include "Vajra/Utilities/CommonDeclarations.h"
#include "Vajra/Utilities/Utilities.h"

#include <map>

// Forward Declarations:
class Camera;
class GameObject;
class RenderLists;

class SceneGraph : public Object, public ITouchTarget {
public:
	virtual ~SceneGraph();

	void Initialize();
	
	void AddNewGameObjectToScene(GameObject* gameObject);
	inline GameObject* GetGameObjectById(ObjectIdType id);
	inline GameObject* GetRootGameObject() { return this->root; }

	void AddGameObjectToRenderLists(GameObject* gameObject);

	Camera* GetMainCamera();
	void SetMainCameraId(ObjectIdType id);

	virtual void update() = 0;
	virtual void draw() = 0;

protected:
	SceneGraph();

	// The default root GameObject which is the ultimate parent of all GameObjects in the scene
	GameObject* root;
	ObjectIdType mainCameraId;

	RenderLists* renderLists;

	virtual void init();
	virtual void destroy();
};


// Inline Functions:

GameObject* SceneGraph::GetGameObjectById(ObjectIdType id) {
	// TODO [Cleanup] Add IF_USING_RTTI to switch between dynamic_cast<> and static_cast<>
	// TODO [Implement] Figure out the best way to ensure that the object returned is of type GameObject
	GameObject* gameObject = dynamic_cast<GameObject*>(ObjectRegistry::GetObjectById(id));
	if (gameObject == nullptr) {
		FRAMEWORK->GetLogger()->dbglog("Warning: GameObject of id %d not found", id);
	} else {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}
	return gameObject;
}

#endif // SCENEGRAPH_H
