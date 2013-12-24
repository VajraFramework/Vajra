#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneGraph/RenderLists.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

SceneGraphUi::SceneGraphUi() : SceneGraph() {
}

SceneGraphUi::~SceneGraphUi() {
	this->destroy();
}

void SceneGraphUi::update() {
	// TODO [Implement] Figure out if we need to put anything here
}

void SceneGraphUi::draw() {
#if 1
	ASSERT(this->mainCameraId != OBJECT_ID_INVALID, "mainCamera set");
	GameObject* mainCamera = this->GetGameObjectById(this->mainCameraId);

	/*
	 * We draw the scene in render-list-iterations
	 * GameObjects which use the same shader program are grouped in the same render list
	 */

	this->renderLists->Begin();
	while (this->renderLists->PrepareCurrentRenderList()) {

		// TODO [Cleanup] Change mainCamera->cameraComponent->WriteLookAt() to use messages sent to mainCamera instead, maybe
		if (mainCamera != nullptr) {
			Camera* cameraComponent = mainCamera->GetComponent<Camera>();
			cameraComponent->WriteLookAt();
		}

		// Render all the renderable game objects in the current render list:
		this->renderLists->RenderGameObjectsInCurrentList();

		this->renderLists->Next();
	}
#endif
}

void SceneGraphUi::init() {
	SceneGraph::init();
	//
	this->mainCameraId = OBJECT_ID_INVALID;
}

void SceneGraphUi::destroy() {
	SceneGraph::destroy();
}
