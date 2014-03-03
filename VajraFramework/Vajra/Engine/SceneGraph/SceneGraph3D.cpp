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

void SceneGraph3D::AddAdditionalLightId(ObjectIdType id) {
	GameObject *directionalLight = this->GetGameObjectById(id);
	ASSERT(directionalLight != nullptr, "New additional directionalLight object not null");
	ASSERT(directionalLight->GetComponent<DirectionalLight>() != nullptr, "New additional directionalLight object has a DirectionalLight component");
	this->additionalLightIds.push_back(id);
}

void SceneGraph3D::update() {
	// TODO [Implement] Figure out if we need to put anything here
}

void SceneGraph3D::draw() {

	GameObject* mainCamera = this->GetGameObjectById(this->mainCameraId);
	if(mainCamera == nullptr) {
		FRAMEWORK->GetLogger()->dbglog("Warning: There is no camera in the SceneGraph3D");
		return;
	}
	GameObject* mainDirectionalLight = this->GetGameObjectById(this->mainDirectionalLightId);
	if(mainDirectionalLight == nullptr) {
		FRAMEWORK->GetLogger()->dbglog("Warning: There is no light in the SceneGraph3D");
		return;
	}

	std::vector<DirectionalLight*> additionalLights_components;
	for (ObjectIdType additionalLightId : this->additionalLightIds) {
		GameObject* additionalLightObject = this->GetGameObjectById(additionalLightId);
		if (additionalLightObject != nullptr) {
			DirectionalLight* additionalLight_component = additionalLightObject->GetComponent<DirectionalLight>();
			additionalLights_components.push_back(additionalLight_component);
		}
	}

	Camera* cameraComponent = mainCamera->GetComponent<Camera>();
	VERIFY(cameraComponent != nullptr, "mainCamera has Camera component");
	DirectionalLight* directionalLightComponent = mainDirectionalLight->GetComponent<DirectionalLight>();
	VERIFY(directionalLightComponent != nullptr, "mainDirectionalLight has DirectionalLight component");

	this->renderLists->Draw(cameraComponent, directionalLightComponent, additionalLights_components);
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

