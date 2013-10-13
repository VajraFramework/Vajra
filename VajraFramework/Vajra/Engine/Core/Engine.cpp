#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Placeholder/Renderer/Renderer.h"

// static member declaration:
Engine* Engine::singleton = nullptr;

Engine::Engine() {
	// Singletons don't call init() in constructor, they call it in GetInstance()
}

Engine::~Engine() {
	this->destroy();
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
	this->messageHub = new MessageHub();
	this->sceneGraph = new SceneGraph();
}

void Engine::UpdateScene() {
	this->GetMessageHub()->DrainMessages();
	this->GetSceneGraph()->update();
}

void Engine::RenderScene() {
	this->GetSceneGraph()->draw();
}

int testEngineFunction() {
    return 80;
}

void Engine::destroy() {
	if (this->messageHub != nullptr) {
		delete this->messageHub;
	}
	if (this->sceneGraph != nullptr) {
		delete this->sceneGraph;
	}
}
