#ifndef ENGINE_H
#define ENGINE_H

int testEngineFunction();

// Forward Declarations:
class AssetLibrary;
class AudioManager;
class DebugDrawer;
class Input;
class MessageHub;
class Profiler;
class SceneGraph;
class Timer;
class Tween;

class Engine {
public:
	~Engine();

	static Engine* GetInstance();

	inline AssetLibrary* GetAssetLibrary() { return this->assetLibrary; }
	inline AudioManager* GetAudioManager() { return this->audioManager; }
	inline DebugDrawer*  GetDebugDrawer()  { return this->debugDrawer;  }
	inline Input*        GetInput()        { return this->input;        }
	inline MessageHub*   GetMessageHub()   { return this->messageHub;   }
	inline Profiler*     GetProfiler()     { return this->profiler;     }
	inline SceneGraph*   GetSceneGraph()   { return this->sceneGraph;   }
	inline Timer*        GetTimer()        { return this->timer;        }
	inline Tween*        GetTween()        { return this->tween;        }

	void DoFrame();

private:
	// Make this a singleton:
	Engine();
	static Engine *singleton;
	//
	void init();
	void destroy();

	void updateInput();
	void updateTweens();
	void updateScene();
	void renderScene();

	AssetLibrary* assetLibrary;
	AudioManager* audioManager;
	DebugDrawer* debugDrawer;
	Input* input;
	MessageHub* messageHub;
	Profiler* profiler;
	SceneGraph* sceneGraph;
	Timer* timer;
	Tween* tween;
};

#define ENGINE Engine::GetInstance()

#endif // ENGINE_H
