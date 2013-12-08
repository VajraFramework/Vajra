#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Engine/SceneGraph/RenderLists.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

SceneGraph::SceneGraph() {
}

SceneGraph::~SceneGraph() {
	this->destroy();
}

void SceneGraph::AddNewGameObjectToScene(GameObject* gameObject) {
	ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());

	ASSERT(gameObject->GetId() > OBJECT_ID_INVALID, "GameObject has valid id: %d", gameObject->GetId());

	FRAMEWORK->GetLogger()->dbglog("\nAdding GameObject with id %d to scene", gameObject->GetId());

	if (gameObject->GetParentId() == OBJECT_ID_INVALID) {
		if (this->root != nullptr) {
			this->root->AddChild(gameObject->GetId());
		}
	}
}

void SceneGraph::AddGameObjectToRenderLists(GameObject* gameObject) {
	std::string shaderName = gameObject->GetShaderName();
	ASSERT_LOG(shaderName != "", "Adding GameObject with id %d to render list %s", gameObject->GetId(), shaderName.c_str());
	this->renderLists->addGameObjectIdToRenderList(gameObject->GetId(), shaderName);
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

void SceneGraph::init() {
	this->root = nullptr;
	this->mainCameraId = OBJECT_ID_INVALID;
	this->mainDirectionalLightId = OBJECT_ID_INVALID;
	this->renderLists = new RenderLists();
}

void SceneGraph::Initialize() {
	this->root = new GameObject();
}

void SceneGraph::destroy() {
}
