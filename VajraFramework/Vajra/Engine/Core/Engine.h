#ifndef ENGINE_H
#define ENGINE_H

int testEngineFunction();

// Forward Declarations:
class SceneGraph;

class Engine {
public:
	static Engine* GetInstance();

	inline SceneGraph* GetSceneGraph() { return this->sceneGraph; }

	void UpdateScene();
	void RenderScene();

private:
	// Make this a singleton:
	Engine();
	static Engine *singleton;
	//
	void init();

	SceneGraph* sceneGraph;
};

#define ENGINE Engine::GetInstance()

#endif // ENGINE_H
