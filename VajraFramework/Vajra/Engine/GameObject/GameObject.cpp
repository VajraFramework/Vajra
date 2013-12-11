#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
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

	this->visible = true;

	FRAMEWORK->GetLogger()->dbglog("\nCreated new GameObject of id: %d", this->GetId());
}

void GameObject::destroy() {
}

void GameObject::Draw() {
	if (this->visible) {
		this->transform->Draw();

		// TODO [Cleanup] Cache the Armature, maybe
		Armature* armature = this->GetComponent<Armature>();
		if (armature != nullptr) {
			armature->Bind();
		}

		// TODO [Cleanup] Cache the MeshRenderer, maybe
		MeshRenderer* meshRenderer = this->GetComponent<MeshRenderer>();
		if (meshRenderer != nullptr) {
			meshRenderer->Draw();
		}
	}
}

std::string GameObject::GetShaderName() {
	// TODO [Cleanup] Cache the MeshRenderer, maybe
	MeshRenderer* meshRenderer = this->GetComponent<MeshRenderer>();
	if (meshRenderer != nullptr) {
		return meshRenderer->GetShaderName();
	}
	return "";
}
