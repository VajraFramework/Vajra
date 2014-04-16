#include "Vajra/Engine/AssetLibrary/AssetLibrary.h"
#include "Vajra/Engine/AudioManager/AudioManager.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/Lighting/AmbientLighting.h"
#include "Vajra/Engine/Lighting/ShadowMap.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/Profiler/Profiler.h"
#include "Vajra/Engine/TagManager/TagManager.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/OpenGL/OpenGLCounter/OpenGLCounter.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
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
	this->timer = new Timer();
	this->timer->init();

	this->messageHub = new MessageHub();
	this->messageHub->init();

	this->assetLibrary = new AssetLibrary();
	this->assetLibrary->init();

	this->ambientLighting = new AmbientLighting();
	this->ambientLighting->init();

	this->sceneGraph3D = new SceneGraph3D();
	this->sceneGraph3D->init();

	this->sceneGraphUi = new SceneGraphUi();
	this->sceneGraphUi->init();

	this->shadowMap = new ShadowMap();
	this->shadowMap->init();

	this->debugDrawer = new DebugDrawer();
	this->debugDrawer->init();

	this->tween = new Tween();
	this->tween->init();

	this->input = new Input();
	this->input->init();

	this->audioManager = new AudioManager();
	this->audioManager->init();

	this->tagManager = new TagManager();
	this->tagManager->init();

	this->profiler = new Profiler();
	this->profiler->init();
}

void Engine::DoFrame() {
	this->GetProfiler()->StartExperiment("frame");
	this->GetTimer()->beginFrame();

	this->updateInput();
	this->updateTweens();
	this->updateScene3D();
	this->updateSceneUi();
	this->renderScene3D();
	this->renderSceneUi();

	FRAMEWORK->GetOpenGLWrapper()->FreeUnusedGLResources();

	FRAMEWORK->GetOpenGLWrapper()->GetOpenGLCounter()->switchFrames();

	this->GetTimer()->endFrame();
	this->GetProfiler()->StopExperiment("frame");
}

void Engine::updateInput() {
	this->GetProfiler()->StartExperiment("input");
	this->GetTimer()->beginInputPhase();

	this->GetInput()->updateInput();

	this->GetTimer()->endInputPhase();
	this->GetProfiler()->StopExperiment("input");
}

void Engine::updateTweens() {
	this->GetProfiler()->StartExperiment("tweens");

	this->GetTween()->updateTweens();

	this->GetProfiler()->StopExperiment("tweens");
}

void Engine::updateScene3D() {
	this->GetProfiler()->StartExperiment("update3d");
	this->GetTimer()->beginUpdatePhase();

	this->GetMessageHub()->drainMessages();
	this->GetSceneGraph3D()->update();

	this->GetTimer()->endUpdatePhase();
	this->GetProfiler()->StopExperiment("update3d");
}

void Engine::renderScene3D() {
	this->GetProfiler()->StartExperiment("render3d");
	this->GetTimer()->beginRenderPhase();

	this->GetSceneGraph3D()->draw();

	this->GetTimer()->endRenderPhase();
	this->GetProfiler()->StopExperiment("render3d");
}

void Engine::updateSceneUi() {
	this->GetProfiler()->StartExperiment("updateUi");
	this->GetTimer()->beginUpdatePhase();

	this->GetMessageHub()->drainMessages();
	this->GetSceneGraphUi()->update();

	this->GetTimer()->endUpdatePhase();
	this->GetProfiler()->StopExperiment("updateUi");
}

void Engine::renderSceneUi() {
	this->GetProfiler()->StartExperiment("renderUi");
	this->GetTimer()->beginRenderPhase();

	this->GetSceneGraphUi()->draw();

	this->GetTimer()->endRenderPhase();
	this->GetProfiler()->StopExperiment("renderUi");
}

int testEngineFunction() {
    return 80;
}

void Engine::destroy() {
	if (this->debugDrawer != nullptr) {
		delete this->debugDrawer;
	}
	if (this->sceneGraph3D != nullptr) {
		delete this->sceneGraph3D;
	}
	if (this->sceneGraphUi != nullptr) {
		delete this->sceneGraphUi;
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
	if (this->ambientLighting != nullptr) {
		delete this->ambientLighting;
	}
	if (this->shadowMap != nullptr) {
		delete this->shadowMap;
	}
	if (this->profiler != nullptr) {
		delete this->profiler;
	}
	if (this->tagManager != nullptr) {
		delete this->tagManager;
	}

}
