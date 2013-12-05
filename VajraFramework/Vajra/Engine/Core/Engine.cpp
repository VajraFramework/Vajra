#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AudioManager/AudioManager.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Engine/Tween/Tween.h"
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
	this->timer        = new Timer();
	this->messageHub   = new MessageHub();
	this->assetLibrary = new AssetLibrary();
	this->sceneGraph   = new SceneGraph();
	this->tween        = new Tween();
	this->input        = new Input();
	this->audioManager = new AudioManager();
}

void Engine::DoFrame() {
	this->GetTimer()->beginFrame();

	this->updateInput();
	this->updateScene();
	this->renderScene();

	this->GetTimer()->endFrame();
}

void Engine::updateInput() {
	this->GetTimer()->beginInputPhase();

	this->GetInput()->updateInput();

	this->GetTimer()->endInputPhase();
}

void Engine::updateScene() {
	this->GetTimer()->beginUpdatePhase();

	this->GetMessageHub()->drainMessages();
	this->GetSceneGraph()->update();

	this->GetTimer()->endUpdatePhase();
}

void Engine::renderScene() {
	this->GetTimer()->beginRenderPhase();

	this->GetSceneGraph()->draw();

	this->GetTimer()->endRenderPhase();
}

int testEngineFunction() {
    return 80;
}

void Engine::destroy() {
	if (this->sceneGraph != nullptr) {
		delete this->sceneGraph;
	}
	if (this->assetLibrary != nullptr) {
		delete this->assetLibrary;
	}
	if (this->messageHub != nullptr) {
		delete this->messageHub;
	}
	if (this->timer != nullptr) {
		delete this->timer;
	}
	if (this->tween != nullptr) {
		delete this->tween;
	}
	if (this->input != nullptr) {
		delete this->input;
	}
	if (this->audioManager != nullptr) {
		delete this->audioManager;
	}
}
