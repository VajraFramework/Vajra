#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Utilities/Utilities.h"
#include "Vajra/Utilities/CommonDeclarations.h"

#include <map>

// Forward Declarations:
class Camera;
class GameObject;

class SceneGraph : public Object {
public:
	~SceneGraph();

	void Initialize();
	
	void AddNewGameObjectToScene(GameObject* gameObject);
	inline GameObject* GetGameObjectById(GameObjectIdType id);
	inline GameObject* GetRootGameObject() { return this->root; }

	Camera* GetMainCamera();
	void SetMainCameraId(GameObjectIdType id);

private:
	SceneGraph();
	void init();
	void destroy();

	void update();
	void draw();

	// The default root GameObject which is the ultimate parent of all GameObjects in the scene
	GameObject* root;

	ObjectIdType mainCameraId;

	friend class Engine;
};

#endif // SCENEGRAPH_H


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
