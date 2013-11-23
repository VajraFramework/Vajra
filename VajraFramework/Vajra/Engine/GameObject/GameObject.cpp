#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/MeshRenderer.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

GameObject::GameObject() {
	this->init();
}

GameObject::~GameObject() {
	this->destroy();
}

void GameObject::init() {
	ENGINE->GetSceneGraph()->AddNewGameObjectToScene(this);

	this->AddComponent<Transform>();
	this->transform = (Transform*)this->GetComponent<Transform>();
	ASSERT(this->transform != 0, "Succeeded in Add and GetComponent of type Transform to GameObject of id: %d", this->GetId());

	FRAMEWORK->GetLogger()->dbglog("\nCreated new GameObject of id: %d", this->GetId());
}

void GameObject::destroy() {
}

void GameObject::Draw() {
	// TODO [Cleanup] Cache the MeshRenderer
	MeshRenderer* meshRenderer = this->GetComponent<MeshRenderer>();
	if (meshRenderer != 0) {
		// TODO [Cleanup] Make it so that we don't have to set the shader set for every GameObject by sorting the GameObjects in the draw calls
		FRAMEWORK->GetOpenGLWrapper()->SetCurrentShaderSet(meshRenderer->GetShaderName());
	}

	this->transform->Draw();
	if (meshRenderer != 0) {
		meshRenderer->Draw();
	}

	// Draw all children:
	for (auto child_it = this->children.begin();
		 child_it != this->children.end(); ++child_it) {

		GameObject* child = ENGINE->GetSceneGraph()->GetGameObjectById(*child_it);
		if (child != 0) {
			child->Draw();
		}
	}
}
