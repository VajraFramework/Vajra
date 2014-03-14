#include "Vajra/Common/Objects/Declarations.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Lighting/AmbientLighting.h"
#include "Vajra/Engine/Lighting/ShadowMap.h"
#include "Vajra/Engine/SceneGraph/RenderLists.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

#include <algorithm>
#include <functional>

////////////////////////////////////////////////////////////////////////////////

static glm::vec3 g_current_cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
static Camera* g_current_camera = nullptr;

bool CompareTrGos(TrGo t1, TrGo t2) {
	//float t1_distanceFromCamera = glm::distance(g_current_cameraPosition, t1.gameobject->GetTransform()->GetPositionWorld());
	//float t2_distanceFromCamera = glm::distance(g_current_cameraPosition, t2.gameobject->GetTransform()->GetPositionWorld());
	float t1_distanceFromCamera = g_current_cameraPosition.z - t1.gameobject->GetTransform()->GetPositionWorld().z; //glm::distance(g_current_cameraPosition, t1.gameobject->GetTransform()->GetPositionWorld());
	float t2_distanceFromCamera = g_current_cameraPosition.z - t2.gameobject->GetTransform()->GetPositionWorld().z;//glm::distance(g_current_cameraPosition, t2.gameobject->GetTransform()->GetPositionWorld());
	return (t1_distanceFromCamera < t2_distanceFromCamera);
}

////////////////////////////////////////////////////////////////////////////////

class RenderList {
public:
	RenderList(std::string shaderName_);

	void Prepare();
	void Draw(HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration* heap_gameobjectsWithTransperancy_out);

	void Draw_one_gameobject(GameObject* gameObject);

	inline std::string GetShaderName() { return this->shaderName; }
	void AddGameObjectId(ObjectIdType id);
	void RemoveGameObjectId(ObjectIdType id);

	bool IsDepthPass();

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

bool RenderList::IsDepthPass() {
	ShaderSet* shaderSet = FRAMEWORK->GetOpenGLWrapper()->GetShaderSetByName(this->GetShaderName());
	VERIFY(shaderSet != nullptr, "Got shader set for renderlist with shader name, %s", this->GetShaderName().c_str());
	return shaderSet->IsDepthPass();
}

void RenderList::Draw(HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration* heap_gameobjectsWithTransperancy_out) {

	if (!this->IsDepthPass()) {

		// Pass the ambient lighting parameters to the shader:
		ENGINE->GetAmbientLighting()->Draw();

		// Pass the shadow map parameters to the shader:
		ENGINE->GetShadowMap()->Draw();
	}

	for (ObjectIdType id : this->gameObjectIds) {
		GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
		if (gameObject != nullptr) {
			if (!gameObject->HasTransperancy() && !gameObject->IsOverlay()) {
				this->Draw_one_gameobject(gameObject);

			} else {

				TrGo trgo;
				trgo.gameobject = gameObject;
				trgo.renderlist = this;
				heap_gameobjectsWithTransperancy_out->push(trgo);

			}
		}
	}
}

void RenderList::Draw_one_gameobject(GameObject* gameObject) {

#if USING_FRUSTUM_CULLING
	if (g_current_camera != nullptr) {
		// TODO [Hack] Get tolerance radius from the model files instead, maybe
		float toleranceRadius = 4.0f;
		if (!g_current_camera->IsPointInFrustum(gameObject->GetTransform()->GetPositionWorld(), toleranceRadius)) {
			return;
		}
	}
#endif

	// Switch off depth buffer for game objects that are supposed to be overlaid on top of everything:
	if (!gameObject->IsOverlay()) {
	} else {
		glDisable(GL_DEPTH_TEST);
	}

	gameObject->Draw();

	// Switch on switched off depth buffer:
	if (!gameObject->IsOverlay()) {
	} else {
		glEnable(GL_DEPTH_TEST);
	}
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

RenderLists::RenderLists() {
	this->init();
}

RenderLists::~RenderLists() {
	this->destroy();
}


void RenderLists::Draw(Camera* camera, bool isDepthPass) {
	std::vector<DirectionalLight*> emptyVector;
	this->Draw(camera, nullptr, emptyVector, isDepthPass);
}

void RenderLists::Draw(Camera* camera, DirectionalLight* directionalLight, std::vector<DirectionalLight*> additionalLights, bool isDepthPass) {

	/*
	 * Switch off blend for opaque objects:
	 */
    glDisable(GL_BLEND);

	/*
	 * We draw the scene in render-list-iterations
	 * GameObjects which use the same shader program are grouped in the same render list
	 * However, transperancy doesn't like to play along.
	 */

	HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration heap_gameobjectsWithTransperancy_out(CompareTrGos);
	if (camera != nullptr) {
		g_current_cameraPosition = ((GameObject*)camera->GetObject())->GetTransform()->GetPositionWorld();
	}

	glm::vec3 left_bottom;
	glm::vec3 right_top;

	this->Begin();
	while (this->PrepareCurrentRenderList()) {


		// If we are drawing a depth pass, process only those renderlists which are depth passes:
		bool currentRenderlistIsDepthPass = this->renderLists[this->currentRenderListIdx]->IsDepthPass();
		if (!(isDepthPass ^ currentRenderlistIsDepthPass)) {

			if (camera != nullptr) {
				g_current_camera = camera;
				// TODO [Cleanup] Change mainCamera->cameraComponent->WriteLookAt() to use messages sent to mainCamera instead, maybe
				camera->WriteLookAt();
			}

			if (directionalLight != nullptr) {
				// TODO [Cleanup] Change directionalLight->WriteLightStuff() to use messages sent to mainDirectionalLight instead, maybe
				directionalLight->WriteLightPropertiesToShader();
			}
			for (DirectionalLight* additionalLight : additionalLights) {
				// TODO [Cleanup] Change additionalLight->WriteLightStuff() to use messages sent to additionalLight instead, maybe
				additionalLight->WriteLightPropertiesToShader();
			}

			// Render all the renderable game objects in the current render list:
			this->RenderGameObjectsInCurrentList(&heap_gameobjectsWithTransperancy_out);

		}

		this->Next();
	}

	/*
	 * Now, render all the transperant game objects in the order of decreasing distance from the camera
	 */
	
	/*
	 * Switch on blend for opaque objects:
	 */
    glEnable(GL_BLEND);
	
	{
		// No transperant objects in depth pass:
		if (!isDepthPass) {

			// Also switch off WRITING to the depth buffer for drawing transperant objects:
			glDepthMask(GL_FALSE);

			while (!heap_gameobjectsWithTransperancy_out.empty()) {
				TrGo trgo = heap_gameobjectsWithTransperancy_out.top();

				trgo.renderlist->Prepare();

				if (camera != nullptr) {
					g_current_camera = camera;
					camera->WriteLookAt();
				}
				if (directionalLight != nullptr) {
					directionalLight->WriteLightPropertiesToShader();
				}
				for (DirectionalLight* additionalLight : additionalLights) {
					// TODO [Cleanup] Change additionalLight->WriteLightStuff() to use messages sent to additionalLight instead, maybe
					additionalLight->WriteLightPropertiesToShader();
				}

				trgo.renderlist->Draw_one_gameobject(trgo.gameobject);

				heap_gameobjectsWithTransperancy_out.pop();
			}

			// Switch WRITING to the depth buffer back on:
			glDepthMask(GL_TRUE);
		}
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

void RenderLists::RenderGameObjectsInCurrentList(HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration* heap_gameobjectsWithTransperancy_out) {

	ASSERT(this->currentRenderListIdx < this->renderLists.size(), "\nCurrent render list idx is valid");
	this->renderLists[this->currentRenderListIdx]->Draw(heap_gameobjectsWithTransperancy_out);
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
	bool found = false;
	for (RenderList* renderList : this->renderLists) {
		if (renderList->GetShaderName() == shaderName) {
			renderList->AddGameObjectId(id);
			found = true;
			break;
		}
	}
	if (!found) {
		FRAMEWORK->GetLogger()->dbglog("\nWARNING: Trying to add game object id %d to render list %s. No such shader render list", id, shaderName.c_str());
		ASSERT(0, "No such shader render list %s", shaderName.c_str());
	}
	// TODO [Hack] Do this better:
	// Also add the game object to depth pass render lists:
	if (shaderName == "bntshdr") {
		this->addGameObjectIdToRenderList(id, "dbtshdr");
	} else if (shaderName == "bncshdr") {
		this->addGameObjectIdToRenderList(id, "dbcshdr");
	} else if (shaderName == "clrshdr") {
		this->addGameObjectIdToRenderList(id, "dpcshdr");
	} else if (shaderName == "txrshdr") {
		this->addGameObjectIdToRenderList(id, "dptshdr");
	}
}

void RenderLists::removeGameObjectIdToRenderList(ObjectIdType id, std::string shaderName) {
	bool found = false;
	for (RenderList* renderList : this->renderLists) {
		if (renderList->GetShaderName() == shaderName) {
			renderList->RemoveGameObjectId(id);
			found = true;
			break;
		}
	}
	if (!found) {
		FRAMEWORK->GetLogger()->dbglog("\nWARNING: Trying to remove game object id %d to render list %s. No such shader render list", id, shaderName.c_str());
		ASSERT(0, "No such shader render list %s", shaderName.c_str());
	}
	// TODO [Hack] Do this better:
	// Also remove the game object from depth pass render lists:
	if (shaderName == "bntshdr") {
		this->removeGameObjectIdToRenderList(id, "dbtshdr");
	} else if (shaderName == "bncshdr") {
		this->removeGameObjectIdToRenderList(id, "dbcshdr");
	} else if (shaderName == "clrshdr") {
		this->removeGameObjectIdToRenderList(id, "dpcshdr");
	} else if (shaderName == "txrshdr") {
		this->removeGameObjectIdToRenderList(id, "dptshdr");
	}
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
