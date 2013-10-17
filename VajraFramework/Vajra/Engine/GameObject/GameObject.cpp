#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/Mesh.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/MeshRenderer.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Framework/Logging/Logger.h"

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

	this->model = new Mesh();

	FRAMEWORK->GetLogger()->dbglog("\nCreated new GameObject of id: %d", this->GetId());
}

void GameObject::destroy() {
}

void GameObject::Draw() {
	// TODO [Cleanup] Cache the MeshRenderer
	this->transform->Draw();
	MeshRenderer* meshRenderer = this->GetComponent<MeshRenderer>();
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
