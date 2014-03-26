#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Lighting/AmbientLighting.h"
#include "Vajra/Engine/Lighting/ShadowMap.h"
#include "Vajra/Engine/SceneGraph/RenderList.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

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

void RenderList::Draw(HEAP_OF_TRANSPERANT_GAMEOBJECTS_declaration* heap_gameobjectsWithTransperancy_out, Camera* camera) {

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
	if (gameObject->GetParentSceneGraph() != (SceneGraph*)ENGINE->GetSceneGraphUi()) {
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

