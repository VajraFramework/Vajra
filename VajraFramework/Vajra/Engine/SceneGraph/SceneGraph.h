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
	void RemoveGameObjectFromRenderLiset(GameObject* gameObject);
	
	void CreateRenderBatches();

	Camera* GetMainCamera();
	void UnsetMainCameraId();
	void SetMainCameraId(ObjectIdType id);

	inline bool IsPaused() { return this->isPaused;  }
	void Pause();
	void Resume();

	virtual void update() = 0;
	virtual void draw() = 0;

	void UnloadCurrentScene();

	void UnbindAllBuffers();

protected:
	SceneGraph();

	// The default root GameObject which is the ultimate parent of all GameObjects in the scene
	GameObject* root;
	ObjectIdType mainCameraId;

	RenderLists* renderLists;

	bool isPaused;

	virtual void init();
	virtual void destroy();
};


// Inline Functions:

GameObject* SceneGraph::GetGameObjectById(ObjectIdType id) {
	GameObject* gameObject = (GameObject*)(ObjectRegistry::GetObjectById(id));
	if (gameObject != nullptr) {
		VERIFY(gameObject->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");
	} else {
		FRAMEWORK->GetLogger()->dbglog("Warning: GameObject of id %d not found", id);
	}
	return gameObject;
}

#endif // SCENEGRAPH_H
