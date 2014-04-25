#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/RenderBatch_static.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/Renderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Lighting/AmbientLighting.h"
#include "Vajra/Engine/Lighting/ShadowMap.h"
#include "Vajra/Engine/SceneGraph/RenderList.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

RenderList::RenderList(std::string shaderName_, SceneGraph* parentScenegraph) {
	this->shaderName = shaderName_;
	this->parentScenegraphRef = parentScenegraph;
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

void RenderList::Draw(HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration* heap_gameobjectsWithTransperancy_out, Camera* camera) {

	if (!this->IsDepthPass()) {
		// Pass the shadow map parameters to the shader:
		ENGINE->GetShadowMap()->Draw();
	}

	for (ObjectIdType id : this->gameObjectIds) {
		GameObject* gameObject = this->parentScenegraphRef->GetGameObjectById(id);
		if (gameObject != nullptr) {
			if (!gameObject->HasTransperancy() && !gameObject->IsOverlay()) {
				this->Draw_one_gameobject(gameObject, camera);

			} else {

				TrGo trgo;
				trgo.gameobject = gameObject;
				trgo.renderlist = this;
				heap_gameobjectsWithTransperancy_out->push(trgo);

			}
		}
	}
}

void RenderList::Draw_one_gameobject(GameObject* gameObject, Camera* camera) {

#if USING_FRUSTUM_CULLING
	// TODO [Hack] This can go away when all renderers support getting their drawable bounds
	// No frustum culling for scenegraph ui:
	// TODO [Hack] No frustum culling for render batches, for now
	// No frustum culling for render batches, for now:
	// TODO [Hack] Also, some renderers can opt out of frustum culling (example: water), this can also go away when all renderers support getting their bounds
	Renderer* renderer = gameObject->GetComponent<Renderer>();
	if (gameObject->GetParentSceneGraph() != (SceneGraph*)ENGINE->GetSceneGraphUi() &&
		!renderer->IsRenderBatch() &&
		!renderer->ShouldPreventCulling()) {
		if (camera != nullptr) {
			// TODO [Hack] Get tolerance radius from the model files instead, maybe
			float toleranceRadius = 4.0f;
			if (!camera->IsPointInFrustum(gameObject->GetTransform()->GetPositionWorld(), toleranceRadius)) {
				return;
			}
		}
	}
#endif

	// Switch off depth buffer for game objects that are supposed to be overlaid on top of everything:
	if (!gameObject->IsOverlay()) {
	} else {
		GLCALL(glDisable, GL_DEPTH_TEST);
	}

	gameObject->Draw();

	// Switch on switched off depth buffer:
	if (!gameObject->IsOverlay()) {
	} else {
		GLCALL(glEnable, GL_DEPTH_TEST);
	}
}

void RenderList::createStaticRenderBatch() {
	/*
	 * Loop through all the game objects in this list and group together
	 * the ones whose renderers are marked static and share the same material as static render batches
	 */

	std::vector<ObjectIdType> staticObjects;

	// Find all the objects that are marked static:
	for (ObjectIdType id : this->gameObjectIds) {
		// TODO [Implement] Handle materials here, but this needs materials to be full assets
		GameObject* gameObject = this->parentScenegraphRef->GetGameObjectById(id);
		if (gameObject != nullptr) {
			Renderer* renderer = gameObject->GetComponent<Renderer>();
			if (renderer != nullptr) {
				if (renderer->IsStatic()) {
					staticObjects.push_back(id);
				}
			}
		}
	}

	if (staticObjects.empty()) {
		// No static objects in render list
		return;
	}

	// Remove static objects from this renderlist:
	for (ObjectIdType id : staticObjects) {
		this->RemoveGameObjectId(id);
	}

	// Create a new empty game object to hold all the static objects in 1 render batch:
	GameObject* renderBatchGameObject = new GameObject(this->parentScenegraphRef);
	RenderBatch_static* renderBatch = renderBatchGameObject->AddComponent<RenderBatch_static>();
	renderBatch->CreateRenderBatchFromMeshes(staticObjects);
	// renderBatchGameObject->GetTransform()->SetPosition(11.0f, 0.0f, -11.0f);
	// renderBatchGameObject->GetTransform()->Scale(5.0f);
}
