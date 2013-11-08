#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/Logging/Logger.h"

SceneGraph::SceneGraph() {
	this->init();
}

SceneGraph::~SceneGraph() {
	this->destroy();
}

void SceneGraph::AddNewGameObjectToScene(GameObject* gameObject) {
	ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());

	ASSERT(gameObject->GetId() > OBJECT_ID_INVALID,
		"GameObject has valid id: %d", gameObject->GetId());

	FRAMEWORK->GetLogger()->dbglog("\nAdding GameObject with id %d to scene", gameObject->GetId());

	if (gameObject->GetParentId() == OBJECT_ID_INVALID) {
		if (this->root != nullptr) {
			this->root->AddChild(gameObject->GetId());
		}
	}
}

// TODO [Cleanup] Cache the mainCamera, maybe
Camera* SceneGraph::GetMainCamera() {
	GameObject *camera = this->GetGameObjectById(this->mainCameraId);
	if (camera != nullptr) {
		return camera->GetComponent<Camera>();
	}
	return nullptr;
}

void SceneGraph::SetMainCameraId(ObjectIdType id) {
	GameObject *camera = this->GetGameObjectById(id);
	ASSERT(camera != nullptr, "New camera object not null");
	ASSERT(camera->GetComponent<Camera>() != nullptr, "New camera object has a camera component");
	this->mainCameraId = id;
}

// TODO [Cleanup] Cache the mainDirectionalLight, maybe
DirectionalLight* SceneGraph::GetMainDirectionalLight() {
	GameObject *directionalLight = this->GetGameObjectById(this->mainDirectionalLightId);
	if (directionalLight != nullptr) {
		return directionalLight->GetComponent<DirectionalLight>();
	}
	return nullptr;
}

void SceneGraph::SetMainDirectionalLightId(ObjectIdType id) {
	GameObject *directionalLight = this->GetGameObjectById(id);
	ASSERT(directionalLight != nullptr, "New directionalLight object not null");
	ASSERT(directionalLight->GetComponent<DirectionalLight>() != nullptr, "New directionalLight object has a DirectionalLight component");
	this->mainDirectionalLightId = id;
}

void SceneGraph::update() {
	// TODO [Implement] Figure out if we need to put anything here
}

void SceneGraph::draw() {

	ASSERT(this->mainCameraId != OBJECT_ID_INVALID, "mainCamera set");
	// TODO [Cleanup] Change mainCamera->cameraComponent->WriteLookAt() to use messages sent to mainCamera instead, maybe
	GameObject* mainCamera = this->GetGameObjectById(this->mainCameraId);
	if (mainCamera != 0) {
		Camera* cameraComponent = mainCamera->GetComponent<Camera>();
		cameraComponent->WriteLookAt();
	}

	ASSERT(this->mainDirectionalLightId != OBJECT_ID_INVALID, "mainDirectionalLight set");
	// TODO [Cleanup] Change mainDirectionalLight->directionalLightComponent->WriteLightStuff() to use messages sent to mainDirectionalLight instead, maybe
	GameObject* mainDirectionalLight = this->GetGameObjectById(this->mainDirectionalLightId);
	if (mainDirectionalLight != 0) {
		DirectionalLight* directionalLightComponent = mainDirectionalLight->GetComponent<DirectionalLight>();
		directionalLightComponent->WriteLightPropertiesToShader();
	}

	if (this->root != 0) {
		this->root->Draw();
	}
}

void SceneGraph::init() {
	this->root = nullptr;
	this->mainCameraId = OBJECT_ID_INVALID;
	this->mainDirectionalLightId = OBJECT_ID_INVALID;
}

void SceneGraph::Initialize() {
	this->root = new GameObject();
}

void SceneGraph::destroy() {
}
