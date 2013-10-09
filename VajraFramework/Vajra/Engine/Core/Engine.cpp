#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Placeholder/Renderer/Renderer.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"

// static member declaration:
Engine* Engine::singleton = nullptr;

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
