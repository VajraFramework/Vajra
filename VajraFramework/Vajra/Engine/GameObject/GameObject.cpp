#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/BakedSkeletalAnimation/BakedSkeletalAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/Renderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/TagManager/TagManager.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

#define NO_TAGS 0

GameObject::GameObject(SceneGraph* sceneGraph) {
	this->init(sceneGraph);
}

GameObject::~GameObject() {
	this->destroy();
}

void GameObject::init(SceneGraph* sceneGraph) {
	sceneGraph->AddNewGameObjectToScene(this);
	this->parentSceneGraph = sceneGraph;

	this->AddComponent<Transform>();
	this->transform = (Transform*)this->GetComponent<Transform>();
	ASSERT(this->transform != 0, "Succeeded in Add and GetComponent of type Transform to GameObject of id: %d", this->GetId());

	this->visible = true;
	this->tags = NO_TAGS;

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

// TODO [Implement] Make it so that the child's invisibility is respected when the parent switches visibility on
void GameObject::SetVisible(bool visible_) {
	this->visible = visible_;
	for (ObjectIdType childId : this->children) {
		GameObject* child = this->GetParentSceneGraph()->GetGameObjectById(childId);
		child->SetVisible(visible_);
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

void GameObject::AddTag(std::string tag) {
	this->AddTag(ENGINE->GetTagManager()->stringToBitmask[tag]);	
}
void GameObject::AddTag(unsigned int tagBit) {
	this->tags = this->tags | tagBit;
}

void GameObject::RemoveTag(std::string tag) {
	this->RemoveTag(ENGINE->GetTagManager()->stringToBitmask[tag]);
}
void GameObject::RemoveTag(unsigned int tagBit) {
	this->tags = ~tagBit & this->tags;
}

bool GameObject::HasTag(std::string tag) {
	return this->HasTag(ENGINE->GetTagManager()->stringToBitmask[tag]);;
}
bool GameObject::HasTag(unsigned int tagBit) {
	return (this->tags & tagBit) > 0;
}

bool GameObject::HasTransperancy() {
	Renderer* renderer = this->GetComponent<Renderer>();
	if (renderer != nullptr) {
		return renderer->HasTransperancy();
	}
	return false;
}
