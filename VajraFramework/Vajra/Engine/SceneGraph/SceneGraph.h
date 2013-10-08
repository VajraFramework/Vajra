#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "Vajra/Utilities/Utilities.h"

#include <map>

// Forward Declarations:
class Camera;
class GameObject;

class SceneGraph {
public:
	~SceneGraph();

	void Initialize();
	
	void AddNewGameObjectToScene(GameObject* gameObject);
	inline GameObject* GetGameObjectById(int id);
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
	// Map of every GameObject in the Scene indexed by id:
	std::map<int /* id */, GameObject*> allGameObjects;

	GameObjectIdType mainCameraId;

	friend class Engine;
};

#endif // SCENEGRAPH_H


// Inline Functions:

GameObject* SceneGraph::GetGameObjectById(int id) {
	auto gameObject_it = this->allGameObjects.find(id);
	if (gameObject_it == this->allGameObjects.end()) {
		return 0;
	}

	return gameObject_it->second;
}
