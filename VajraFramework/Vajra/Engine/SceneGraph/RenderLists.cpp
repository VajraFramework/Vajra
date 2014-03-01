#include "Vajra/Common/Objects/Declarations.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Lighting/AmbientLighting.h"
#include "Vajra/Engine/SceneGraph/RenderLists.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"

#include <algorithm>
#include <functional>

////////////////////////////////////////////////////////////////////////////////

static glm::vec3 g_cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
static Camera* g_camera = nullptr;

bool CompareTrGos(TrGo t1, TrGo t2) {
	float t1_distanceFromCamera = glm::distance(g_cameraPosition, t1.gameobject->GetTransform()->GetPosition());
	float t2_distanceFromCamera = glm::distance(g_cameraPosition, t2.gameobject->GetTransform()->GetPosition());
	return (t1_distanceFromCamera < t2_distanceFromCamera);
}

////////////////////////////////////////////////////////////////////////////////

class RenderList {
public:
	RenderList(std::string shaderName_);

	void Prepare();
	void Draw(HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration* heap_gameobjectsWithTransperancy_out,
		float x_min_bound, float x_max_bound, float y_min_bound, float y_max_bound, float z_min_bound, float z_max_bound);

	void Draw_one_gameobject(GameObject* gameObject,
		float x_min_bound, float x_max_bound, float y_min_bound, float y_max_bound, float z_min_bound, float z_max_bound);

	inline std::string GetShaderName() { return this->shaderName; }
	void AddGameObjectId(ObjectIdType id);
	void RemoveGameObjectId(ObjectIdType id);

private:
	std::string shaderName;
	std::vector<ObjectIdType> gameObjectIds;
};

RenderList::RenderList(std::string shaderName_) {
	this->shaderName = shaderName_;
}

void RenderList::AddGameObjectId(ObjectIdType id) {
	// TODO [Implement] Make sure there are no duplicates
	// TODO [Implement] Keep this sorted so that insertions w/o duplicates and deletions are faster
	this->gameObjectIds.push_back(id);
}

void RenderList::RemoveGameObjectId(ObjectIdType id) {
	// TODO [Implement] Make sure there are no duplicates
	// TODO [Implement] Keep this sorted so that insertions w/o duplicates and deletions are faster
	auto id_it = std::find(this->gameObjectIds.begin(), this->gameObjectIds.end(), id);
	if (id_it != this->gameObjectIds.end()) {
		this->gameObjectIds.erase(id_it);
	} else {
		FRAMEWORK->GetLogger()->dbglog("\nTrying to remove game object id %d from renderlist %s. No such game object id", id, this->shaderName.c_str());
	}
}


void RenderList::Prepare() {
	FRAMEWORK->GetOpenGLWrapper()->SetCurrentShaderSet(this->GetShaderName());
}

void RenderList::Draw(HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration* heap_gameobjectsWithTransperancy_out,
					  float x_min_bound, float x_max_bound, float y_min_bound, float y_max_bound, float z_min_bound, float z_max_bound) {

	// Pass the ambient lighting parameters to the shader:
	ENGINE->GetAmbientLighting()->Draw();

	for (ObjectIdType id : this->gameObjectIds) {
		GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
		if (gameObject != nullptr) {
			if (!gameObject->HasTransperancy()) {
				this->Draw_one_gameobject(gameObject, x_min_bound, x_max_bound, y_min_bound, y_max_bound, z_min_bound, z_max_bound);

			} else {

				TrGo trgo;
				trgo.gameobject = gameObject;
				trgo.renderlist = this;
				heap_gameobjectsWithTransperancy_out->push(trgo);

			}
		}
	}
}

void RenderList::Draw_one_gameobject(GameObject* gameObject,
	 float x_min_bound, float x_max_bound, float y_min_bound, float y_max_bound, float z_min_bound, float z_max_bound) {

	glm::vec3 gameObjectPosition = gameObject->GetTransform()->GetPositionWorld();

	if (gameObjectPosition.x < x_min_bound || gameObjectPosition.x > x_max_bound ||
		gameObjectPosition.y < y_min_bound || gameObjectPosition.y > y_max_bound ||
		gameObjectPosition.z < z_min_bound || gameObjectPosition.z > z_max_bound) {
		// Outside viewable volume:
		// return;
	}

	if (g_camera != nullptr) {
		if (!g_camera->IsPointInFrustum(gameObject->GetTransform()->GetPositionWorld())) {
			return;
		}
	}

	gameObject->Draw();
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

RenderLists::RenderLists() {
	this->init();
}

RenderLists::~RenderLists() {
	this->destroy();
}


void RenderLists::Draw(Camera* camera, DirectionalLight* directionalLight /* = nullptr */) {

	/*
	 * We draw the scene in render-list-iterations
	 * GameObjects which use the same shader program are grouped in the same render list
	 * However, transperancy doesn't like to play along.
	 */

	HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration heap_gameobjectsWithTransperancy_out(CompareTrGos);
	if (camera != nullptr) {
		g_cameraPosition = ((GameObject*)camera->GetObject())->GetTransform()->GetPosition();
	}

	glm::vec3 left_bottom;
	glm::vec3 right_top;

	this->Begin();
	while (this->PrepareCurrentRenderList()) {

		if (camera != nullptr) {
			g_camera = camera;
			// TODO [Cleanup] Change mainCamera->cameraComponent->WriteLookAt() to use messages sent to mainCamera instead, maybe
			camera->WriteLookAt();
		}
		if (directionalLight != nullptr) {
			// TODO [Cleanup] Change mainDirectionalLight->directionalLightComponent->WriteLightStuff() to use messages sent to mainDirectionalLight instead, maybe
			directionalLight->WriteLightPropertiesToShader();
		}

		// Get bounds of viewable area:
		// left_bottom = camera->ScreenToWorldPoint( glm::vec3(1.0f, FRAMEWORK->GetDeviceProperties()->GetHeightPixels(), 1.0f) );
		// right_top   = camera->ScreenToWorldPoint( glm::vec3(FRAMEWORK->GetDeviceProperties()->GetWidthPixels(),  1.0f, 1.0f) );

		// Render all the renderable game objects in the current render list:
		this->RenderGameObjectsInCurrentList(&heap_gameobjectsWithTransperancy_out,
				left_bottom.x, right_top.x, -9999.9f, 9999.9f, left_bottom.z, right_top.z);

		this->Next();
	}


	/*
	 * Now, render all the transperant game objects in the order of decreasing distance from the camera
	 */

	while (!heap_gameobjectsWithTransperancy_out.empty()) {
		TrGo trgo = heap_gameobjectsWithTransperancy_out.top();

		trgo.renderlist->Prepare();

		if (camera != nullptr) {
			g_camera = camera;
			camera->WriteLookAt();
		}
		if (directionalLight != nullptr) {
			directionalLight->WriteLightPropertiesToShader();
		}

		trgo.renderlist->Draw_one_gameobject(trgo.gameobject,
				left_bottom.x, right_top.x, -9999.9f, 9999.9f, left_bottom.z, right_top.z);

		heap_gameobjectsWithTransperancy_out.pop();
	}
}

void RenderLists::Begin() {
	this->currentRenderListIdx = 0;
}

void RenderLists::Next() {
	this->currentRenderListIdx++;
}

bool RenderLists::PrepareCurrentRenderList() {
	if (this->currentRenderListIdx < this->renderLists.size()) {
		this->renderLists[this->currentRenderListIdx]->Prepare();
		return true;
	}
	return false;
}

void RenderLists::RenderGameObjectsInCurrentList(HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration* heap_gameobjectsWithTransperancy_out,
		float x_min_bound, float x_max_bound, float y_min_bound, float y_max_bound, float z_min_bound, float z_max_bound) {

	ASSERT(this->currentRenderListIdx < this->renderLists.size(), "\nCurrent render list idx is valid");
	this->renderLists[this->currentRenderListIdx]->Draw(heap_gameobjectsWithTransperancy_out,
			x_min_bound, x_max_bound, y_min_bound, y_max_bound, z_min_bound, z_max_bound);
}

void RenderLists::init() {
	this->currentRenderListIdx = 0;
	this->createRenderLists();
}

void RenderLists::destroy() {
	for (unsigned int i = 0; i < this->renderLists.size(); ++i) {
		RenderList* renderList = this->renderLists[i];
		delete renderList;
	}
	this->renderLists.clear();
}

void RenderLists::addGameObjectIdToRenderList(ObjectIdType id, std::string shaderName) {
	for (RenderList* renderList : this->renderLists) {
		if (renderList->GetShaderName() == shaderName) {
			renderList->AddGameObjectId(id);
			return;
		}
	}
	FRAMEWORK->GetLogger()->dbglog("\nWARNING: Trying to add game object id %d to render list %s. No such shader render list", id, shaderName.c_str());
}

void RenderLists::removeGameObjectIdToRenderList(ObjectIdType id, std::string shaderName) {
	for (RenderList* renderList : this->renderLists) {
		if (renderList->GetShaderName() == shaderName) {
			renderList->RemoveGameObjectId(id);
			return;
		}
	}
	FRAMEWORK->GetLogger()->dbglog("\nWARNING: Trying to remove game object id %d to render list %s. No such shader render list", id, shaderName.c_str());
}


void RenderLists::createRenderLists() {
	ASSERT_LOG(this->renderLists.size() == 0, "Creating RenderLists for the first time");

	std::vector<std::string> shaderNames;
	FRAMEWORK->GetOpenGLWrapper()->GetAllAvailableShaderNames(shaderNames);

	for (std::string shaderName : shaderNames) {
		RenderList* newRenderList = new RenderList(shaderName);
		this->renderLists.push_back(newRenderList);
	}

	FRAMEWORK->GetLogger()->dbglog("\nCreated %d render lists", this->renderLists.size());
}
