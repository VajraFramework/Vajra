#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Engine/SceneGraph/RenderLists.h"
#include "Vajra/Engine/Tween/Tween.h"
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

void SceneGraph::RemoveGameObjectFromRenderLiset(GameObject* gameObject) {
	std::string shaderName = gameObject->GetShaderName();
	ASSERT_LOG(shaderName != "", "Removing GameObject with id %d to render list %s", gameObject->GetId(), shaderName.c_str());
	this->renderLists->removeGameObjectIdToRenderList(gameObject->GetId(), shaderName);
}
// TODO [Cleanup] Cache the mainCamera, maybe
Camera* SceneGraph::GetMainCamera() {
	if (this->mainCameraId != OBJECT_ID_INVALID) {
		GameObject *camera = this->GetGameObjectById(this->mainCameraId);
		if (camera != nullptr) {
			return camera->GetComponent<Camera>();
		}
	}
	return nullptr;
}

void SceneGraph::UnsetMainCameraId() {
	this->mainCameraId = OBJECT_ID_INVALID;
}

void SceneGraph::SetMainCameraId(ObjectIdType id) {
	GameObject *camera = this->GetGameObjectById(id);
	ASSERT(camera != nullptr, "New camera object not null");
	ASSERT(camera->GetComponent<Camera>() != nullptr, "New camera object has a camera component");
	this->mainCameraId = id;
}

void SceneGraph::UnloadCurrentScene() {
	this->root->destroyAllChildren();

	delete this->renderLists; 
	this->renderLists = new RenderLists(this);
}

void SceneGraph::Pause() {
	this->isPaused = true;
	this->root->Pause();

	ENGINE->GetTween()->PauseNumberTweensByAffiliation(NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D);
}

void SceneGraph::Resume() {
	this->isPaused = false;
	this->root->Resume();

	ENGINE->GetTween()->ResumeNumberTweensByAffiliation(NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D);
}

void SceneGraph::CreateRenderBatches() {
	this->renderLists->createStaticRenderBatches();
}

void SceneGraph::init() {
	this->root = nullptr;
	this->renderLists = new RenderLists(this);
	this->isPaused = false;
}

void SceneGraph::Initialize() {
	this->root = new GameObject(this);
}

void SceneGraph::UnbindAllBuffers() {
	this->renderLists->UnbindAllBuffers();
}


void SceneGraph::destroy() {
}
