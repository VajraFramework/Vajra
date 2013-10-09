#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
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
	ASSERT(gameObject->GetId() > GAMEOBJECT_ID_INVALID,
		"GameObject has valid id: %d", gameObject->GetId());
	ASSERT(this->GetGameObjectById(gameObject->GetId()) == nullptr,
		"GameObject has unique id: %d", gameObject->GetId());

	FRAMEWORK->GetLogger()->dbglog("\nAdding GameObject with id %d to scene", gameObject->GetId());

	// Add this GameObject to the map of all GameObjects:
	this->allGameObjects[gameObject->GetId()] = gameObject;
}

// TODO [Cleanup] Cache the mainCamera, maybe
Camera* SceneGraph::GetMainCamera() {
	GameObject *camera = this->GetGameObjectById(this->mainCameraId);
	if (camera != nullptr) {
		return camera->GetComponent<Camera>();
	}
	return nullptr;
}

void SceneGraph::SetMainCameraId(GameObjectIdType id) {
	GameObject *camera = this->GetGameObjectById(id);
	ASSERT(camera != nullptr, "New camera object not null");
	ASSERT(camera->GetComponent<Camera>() != nullptr, "New camera object has a camera component");
	this->mainCameraId = id;
}

void SceneGraph::update() {
	if (this->root != 0) {
		this->root->Update();
	}
}

void SceneGraph::draw() {
	ASSERT(this->mainCameraId != GAMEOBJECT_ID_INVALID, "mainCamera set");
	// TODO [Cleanup] Change mainCamera->cameraComponent->WriteLookAt() to use messages sent to mainCamera instead, maybe
	GameObject* mainCamera = this->GetGameObjectById(this->mainCameraId);
	if (mainCamera != 0) {
		Camera* cameraComponent = mainCamera->GetComponent<Camera>();
		cameraComponent->WriteLookAt();
	}
	if (this->root != 0) {
		this->root->Draw();
	}
}

void SceneGraph::init() {
	this->root = 0;
	this->mainCameraId = GAMEOBJECT_ID_INVALID;
}

void SceneGraph::Initialize() {
	this->root = new GameObject();
}

void SceneGraph::destroy() {
}
