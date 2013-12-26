#ifndef RENDER_LISTS_H
#define RENDER_LISTS_H

#include <map>
#include <string>
#include <vector>

// Forward Declarations:
class RenderList;

class RenderLists {
public:
	~RenderLists();

	void Begin();
	void Next();

	bool PrepareCurrentRenderList();

	void RenderGameObjectsInCurrentList();

private:
	RenderLists();
	void init();
	void destroy();

	void addGameObjectIdToRenderList(ObjectIdType id, std::string shaderName);

	// Utility Functions:
	void createRenderLists();

	unsigned int currentRenderListIdx;
	std::vector<RenderList*> renderLists;

	// friended to SceneGraph so as to not have to expose the constructor and addGameObjectIdToRenderList()
	friend class SceneGraph;
	friend class SceneGraph3D;
};

#endif // RENDER_LISTS_H
