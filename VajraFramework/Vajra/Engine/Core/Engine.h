#ifndef ENGINE_H
#define ENGINE_H

int testEngineFunction();

// Forward Declarations:
class MessageHub;
class SceneGraph;
class Timer;

class Engine {
public:
	~Engine();

	static Engine* GetInstance();

	inline MessageHub* GetMessageHub() { return this->messageHub; }
	inline SceneGraph* GetSceneGraph() { return this->sceneGraph; }
	inline Timer*      GetTimer()      { return this->timer;      }

	void UpdateScene();
	void RenderScene();

private:
	// Make this a singleton:
	Engine();
	static Engine *singleton;
	//
	void init();
	void destroy();

	MessageHub* messageHub;
	SceneGraph* sceneGraph;
	Timer* timer;
};

#define ENGINE Engine::GetInstance()

#endif // ENGINE_H
