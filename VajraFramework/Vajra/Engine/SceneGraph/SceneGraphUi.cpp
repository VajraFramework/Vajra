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

// TODO [Cleanup] Cache the mainCamera, maybe
Camera* SceneGraphUi::GetMainCamera() {
	GameObject *camera = this->GetGameObjectById(this->mainCameraId);
	if (camera != nullptr) {
		return camera->GetComponent<Camera>();
	}
	return nullptr;
}

void SceneGraphUi::SetMainCameraId(ObjectIdType id) {
	GameObject *camera = this->GetGameObjectById(id);
	ASSERT(camera != nullptr, "New camera object not null");
	Camera* cameraComponent = camera->GetComponent<Camera>();
	ASSERT(cameraComponent != nullptr, "New camera object has a camera component");
	// TODO [Cleanup] This should become unnecessary once better ways of defining the ui are in place
	ASSERT(cameraComponent->GetCameraType() == CAMERA_TYPE_ORTHO, "Ui Camera is of type ortho");
	this->mainCameraId = id;
}

void SceneGraphUi::update() {
	// TODO [Implement] Figure out if we need to put anything here
}

void SceneGraphUi::draw() {

#if 0
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
