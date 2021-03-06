#include "Vajra/Common/Objects/Declarations.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Lighting/AmbientLighting.h"
#include "Vajra/Engine/Lighting/ShadowMap.h"
#include "Vajra/Engine/SceneGraph/RenderList.h"
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

bool CompareTrGos_ortho_z(TrGo t1, TrGo t2) {
	float t1_distanceFromCamera = g_current_cameraPosition.z - t1.gameobject->GetTransform()->GetPositionWorld().z;
	float t2_distanceFromCamera = g_current_cameraPosition.z - t2.gameobject->GetTransform()->GetPositionWorld().z;
	return (t1_distanceFromCamera < t2_distanceFromCamera);
}

bool CompareTrGos_perspective(TrGo t1, TrGo t2) {
	float t1_distanceFromCamera = glm::distance(g_current_cameraPosition, t1.gameobject->GetTransform()->GetPositionWorld());
	float t2_distanceFromCamera = glm::distance(g_current_cameraPosition, t2.gameobject->GetTransform()->GetPositionWorld());
	return (t1_distanceFromCamera < t2_distanceFromCamera);
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////

RenderLists::RenderLists(SceneGraph* parentScenegraph) {
	this->init(parentScenegraph);
}

RenderLists::~RenderLists() {
	this->destroy();
}


void RenderLists::UnbindAllBuffers() {
	this->Begin();
	while (this->PrepareCurrentRenderList()) {

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();
		// No normals, tangents, bitangents, or texture coordinates in depth pass:
		if (!currentShaderSet->IsDepthPass()) {
			if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_vNormal)) {
				GLint handle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_vNormal);
				GLCALL(glDisableVertexAttribArray, handle);
			}
			if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_vTangent)) {
				GLint handle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_vTangent);
				GLCALL(glDisableVertexAttribArray, handle);
			}
			if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_vBitangent)) {
				GLint handle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_vBitangent);
				GLCALL(glDisableVertexAttribArray, handle);
			}
			if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_uvCoords_in)) {
				GLint handle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_uvCoords_in);
				GLCALL(glDisableVertexAttribArray, handle);
			}
		}

		this->Next();
	}
}


void RenderLists::Draw(Camera* camera, bool isDepthPass, DistanceFromCameraCompareType compareType) {
	std::vector<DirectionalLight*> emptyVector;
	this->Draw(camera, nullptr, emptyVector, isDepthPass, compareType);
}

void RenderLists::Draw(Camera* camera, DirectionalLight* directionalLight, std::vector<DirectionalLight*> additionalLights, bool isDepthPass, DistanceFromCameraCompareType compareType) {

	/*
	 * Switch off blend for opaque objects:
	 */
    GLCALL(glDisable, GL_BLEND);

	/*
	 * We draw the scene in render-list-iterations
	 * GameObjects which use the same shader program are grouped in the same render list
	 * However, transperancy doesn't like to play along.
	 */

	HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration* heap_gameobjectsWithTransperancy_out;
	if (compareType == DISTANCE_FROM_CAMERA_COMPARE_TYPE_perspective) {
		heap_gameobjectsWithTransperancy_out = new HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration(CompareTrGos_perspective);
	} else if (compareType == DISTANCE_FROM_CAMERA_COMPARE_TYPE_ortho_z) {
		heap_gameobjectsWithTransperancy_out = new HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration(CompareTrGos_ortho_z);
	} else {
		ASSERT(0, "Valid compare type, %d", compareType);
	}

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
			this->RenderGameObjectsInCurrentList(heap_gameobjectsWithTransperancy_out);

		}

		this->Next();
	}

	/*
	 * Now, render all the transperant game objects in the order of decreasing distance from the camera
	 */
	
	/*
	 * Switch on blend for opaque objects:
	 */
    GLCALL(glEnable, GL_BLEND);
	
	{
		// No transperant objects in depth pass:
		if (!isDepthPass) {

			// Also switch off WRITING to the depth buffer for drawing transperant objects:
			GLCALL(glDepthMask, GL_FALSE);

			while (!heap_gameobjectsWithTransperancy_out->empty()) {
				TrGo trgo = heap_gameobjectsWithTransperancy_out->top();

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

				trgo.renderlist->Draw_one_gameobject(trgo.gameobject, g_current_camera);

				heap_gameobjectsWithTransperancy_out->pop();
			}

			// Switch WRITING to the depth buffer back on:
			GLCALL(glDepthMask, GL_TRUE);
		}
	}

	delete heap_gameobjectsWithTransperancy_out;
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
	this->renderLists[this->currentRenderListIdx]->Draw(heap_gameobjectsWithTransperancy_out, g_current_camera);
}

void RenderLists::init(SceneGraph* parentScenegraph) {
	this->parentScenegraphRef = parentScenegraph;

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
	} else if (shaderName == "wtrshdr") {
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
	} else if (shaderName == "wtrshdr") {
		this->removeGameObjectIdToRenderList(id, "dptshdr");
	}
}


void RenderLists::createRenderLists() {
	ASSERT_LOG(this->renderLists.size() == 0, "Creating RenderLists for the first time");

	std::vector<std::string> shaderNames;
	FRAMEWORK->GetOpenGLWrapper()->GetAllAvailableShaderNames(shaderNames);

	for (std::string shaderName : shaderNames) {
		RenderList* newRenderList = new RenderList(shaderName, this->parentScenegraphRef);
		this->renderLists.push_back(newRenderList);
	}

	FRAMEWORK->GetLogger()->dbglog("\nCreated %d render lists", this->renderLists.size());
}

void RenderLists::createStaticRenderBatches() {
#if USING_STATIC_RENDER_BATCHING
	/*
	 * Go through all the render lists and batch together objects in the same render list
	 * that are marked static
	 */
	for (RenderList* renderList : this->renderLists) {
		renderList->createStaticRenderBatch();
	}
#endif
}

