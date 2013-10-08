#ifndef ENGINE_H
#define ENGINE_H

int testEngineFunction();

// Forward Declarations:
class Logger;
class OpenGLWrapper;
class SceneGraph;
class FileSystemUtils;

class Engine {
public:
	static Engine* GetInstance();

	inline Logger* GetLogger() { return this->logger; }
    inline FileSystemUtils* GetFileSystemUtils() { return this->fileSystemUtils; }
	inline OpenGLWrapper* GetOpenGLWrapper() { return this->openGLWrapper; }
	inline SceneGraph* GetSceneGraph() { return this->sceneGraph; }

	void UpdateScene();
	void RenderScene();

private:
	// Make this a singleton:
	Engine();
	static Engine *singleton;
	//
	void init();

	Logger* logger;
    FileSystemUtils* fileSystemUtils;
	OpenGLWrapper* openGLWrapper;
	SceneGraph* sceneGraph;

};

#define ENGINE Engine::GetInstance()

#endif // ENGINE_H