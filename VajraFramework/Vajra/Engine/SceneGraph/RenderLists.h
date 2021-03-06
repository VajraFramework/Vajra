#ifndef RENDER_LISTS_H
#define RENDER_LISTS_H

#include "Vajra/Common/Objects/Object.h"

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#include <functional>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>

// Forward Declarations:
class Camera;
class DirectionalLight;
class GameObject;
class RenderList;

#define USING_FRUSTUM_CULLING 1
#define USING_STATIC_RENDER_BATCHING 1

// TODO [Hack] For some reason the simulator crashes in glDrawElements() when we do this
#ifdef __APPLE__
#if TARGET_IPHONE_SIMULATOR
#define USING_STATIC_RENDER_BATCHING 0
#endif
#endif

////////////////////////////////////////////////////////////////////////////////

// Transperant GameObject:
class TrGo {
public:
	RenderList* renderlist;
	GameObject* gameobject;
};

enum DistanceFromCameraCompareType {
	DISTANCE_FROM_CAMERA_COMPARE_TYPE_perspective,
	DISTANCE_FROM_CAMERA_COMPARE_TYPE_ortho_z,
};

bool CompareTrGos(TrGo t1, TrGo t2);

#define HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration std::priority_queue< TrGo, std::vector<TrGo>, std::function<bool(TrGo, TrGo)> >

////////////////////////////////////////////////////////////////////////////////

class RenderLists {
public:
	~RenderLists();

	void Draw(Camera* camera, bool isDepthPass, DistanceFromCameraCompareType compareType);
	void Draw(Camera* camera, DirectionalLight* directionalLight, std::vector<DirectionalLight*> additionalLights, bool isDepthPass, DistanceFromCameraCompareType compareType);

	void UnbindAllBuffers();

private:
	RenderLists(SceneGraph* parentScenegraph);
	void init(SceneGraph* parentScenegraph);
	void destroy();

	void Begin();
	void Next();
	void SetCurrentRenderListIdx(unsigned int idx) { this->currentRenderListIdx = idx; }
	bool PrepareCurrentRenderList();
	void RenderGameObjectsInCurrentList(HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration* heap_gameobjectsWithTransperancy_out);

	void addGameObjectIdToRenderList(ObjectIdType id, std::string shaderName);
	void removeGameObjectIdToRenderList(ObjectIdType id, std::string shaderName);

	void createStaticRenderBatches();

	// Utility Functions:
	void createRenderLists();

	unsigned int currentRenderListIdx;
	std::vector<RenderList*> renderLists;

	SceneGraph* parentScenegraphRef;

	// friended to SceneGraph so as to not have to expose the constructor and addGameObjectIdToRenderList()
	friend class SceneGraph;
	friend class SceneGraph3D;
};

#endif // RENDER_LISTS_H
