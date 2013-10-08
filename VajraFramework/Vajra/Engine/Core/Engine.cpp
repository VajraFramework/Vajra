#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Placeholder/Renderer/Renderer.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/Logging/Logger.h"

// static member declaration:
Engine* Engine::singleton = 0;

Engine::Engine() {
	// Singletons don't call init() in constructor, they call it in GetInstance()
}

// static method
Engine* Engine::GetInstance() {
	if (Engine::singleton == 0) {
		Engine::singleton = new Engine();
		Engine::singleton->init();
	}

	return Engine::singleton;
}

void Engine::init() {
	// Do not sort the following in any arbitrary order:
	this->logger = new Logger();
    this->fileSystemUtils = new FileSystemUtils();
	this->openGLWrapper = new OpenGLWrapper();
	this->sceneGraph = new SceneGraph();
}

void Engine::UpdateScene() {
	this->GetSceneGraph()->update();
}

void Engine::RenderScene() {
	this->GetSceneGraph()->draw();
}

int testEngineFunction() {
    return 80;
}
