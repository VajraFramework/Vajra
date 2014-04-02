#ifndef RENDERLIST_H
#define RENDERLIST_H

#include "Vajra/Engine/SceneGraph/RenderLists.h"

#include <functional>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>

// Forward Declarations:
class Camera;
class GameObject;
class SceneGraph;

class RenderList {
public:
	RenderList(std::string shaderName_, SceneGraph* parentScenegraph);

	void Prepare();
	void Draw(HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration* heap_gameobjectsWithTransperancy_out, Camera* camera);

	void Draw_one_gameobject(GameObject* gameObject, Camera* camera);

	inline std::string GetShaderName() { return this->shaderName; }
	void AddGameObjectId(ObjectIdType id);
	void RemoveGameObjectId(ObjectIdType id);

	bool IsDepthPass();

	void createStaticRenderBatch();

private:
	std::string shaderName;
	std::vector<ObjectIdType> gameObjectIds;

	SceneGraph* parentScenegraphRef;
};

#endif // RENDERLIST_H
