#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/RenderLists.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

SceneGraph3D::SceneGraph3D() : SceneGraph() {
}

SceneGraph3D::~SceneGraph3D() {
	this->destroy();
}

// TODO [Cleanup] Cache the mainDirectionalLight, maybe
DirectionalLight* SceneGraph3D::GetMainDirectionalLight() {
	GameObject *directionalLight = this->GetGameObjectById(this->mainDirectionalLightId);
	if (directionalLight != nullptr) {
		return directionalLight->GetComponent<DirectionalLight>();
	}
	return nullptr;
}

void SceneGraph3D::SetMainDirectionalLightId(ObjectIdType id) {
	GameObject *directionalLight = this->GetGameObjectById(id);
	ASSERT(directionalLight != nullptr, "New directionalLight object not null");
	ASSERT(directionalLight->GetComponent<DirectionalLight>() != nullptr, "New directionalLight object has a DirectionalLight component");
	this->mainDirectionalLightId = id;
}

void SceneGraph3D::update() {
	// TODO [Implement] Figure out if we need to put anything here
}

void SceneGraph3D::draw() {

	ASSERT(this->mainCameraId != OBJECT_ID_INVALID, "mainCamera set");
	GameObject* mainCamera = this->GetGameObjectById(this->mainCameraId);
	ASSERT(this->mainDirectionalLightId != OBJECT_ID_INVALID, "mainDirectionalLight set");
	GameObject* mainDirectionalLight = this->GetGameObjectById(this->mainDirectionalLightId);

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
		//
		// TODO [Cleanup] Change mainDirectionalLight->directionalLightComponent->WriteLightStuff() to use messages sent to mainDirectionalLight instead, maybe
		if (mainDirectionalLight != nullptr) {
			DirectionalLight* directionalLightComponent = mainDirectionalLight->GetComponent<DirectionalLight>();
			directionalLightComponent->WriteLightPropertiesToShader();
		}

		// Render all the renderable game objects in the current render list:
		this->renderLists->RenderGameObjectsInCurrentList();

		this->renderLists->Next();
	}
}

void SceneGraph3D::init() {
	SceneGraph::init();
	//
	this->mainCameraId = OBJECT_ID_INVALID;
	this->mainDirectionalLightId = OBJECT_ID_INVALID;
}

void SceneGraph3D::destroy() {
	SceneGraph::destroy();
}
