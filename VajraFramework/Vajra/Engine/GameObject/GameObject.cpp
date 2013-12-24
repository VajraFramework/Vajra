#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/BakedSkeletalAnimation/BakedSkeletalAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/Renderer.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
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
	ENGINE->GetSceneGraph3D()->AddNewGameObjectToScene(this);

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

#if USING_RUNTIME_COMPUTED_BONE_MATRICES
		// TODO [Cleanup] Cache the Armature, maybe
		Armature* armature = this->GetComponent<Armature>();
		if (armature != nullptr) {
			armature->Bind();
		}
#elif USING_BAKED_BONE_MATRICES
		// TODO [Cleanup] Cache the BakedSkeletalAnimation component, maybe
		BakedSkeletalAnimation* bakedSkeletalAnimation = this->GetComponent<BakedSkeletalAnimation>();
		if (bakedSkeletalAnimation != nullptr) {
			bakedSkeletalAnimation->Bind();
		}
#endif

		// TODO [Cleanup] Cache the Renderer, maybe
		Renderer* renderer = this->GetComponent<Renderer>();
		if (renderer != nullptr) {
			renderer->Draw();
		}
	}
}

std::string GameObject::GetShaderName() {
	// TODO [Cleanup] Cache the Renderer, maybe
	Renderer* renderer = this->GetComponent<Renderer>();
	if (renderer != nullptr) {
		return renderer->GetShaderName();
	}
	return "";
}
