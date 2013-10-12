#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/BaseComponent/Component.h"
#include "Vajra/Engine/Objects/GameObject/GameObject.h"
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
	this->id = this->getNextFreeId();
	ENGINE->GetSceneGraph()->AddNewGameObjectToScene(this);

	this->AddComponent<Transform>();
	this->transform = (Transform*)this->GetComponent<Transform>();
	ASSERT(this->transform != 0, "Succeeded in Add and GetComponent of type Transform to GameObject of id: %d", this->GetId());

	this->model = new Mesh();

	FRAMEWORK->GetLogger()->dbglog("\nCreated new GameObject of id: %d", this->GetId());
}

void GameObject::destroy() {
	this->destroyAllChildren();
	this->removeAllComponents();
}

void GameObject::Update() {
	// Update all attached Components:
	for (auto component_it = this->componentMap.begin();
		 component_it != this->componentMap.end(); ++component_it) {

		component_it->second->Update();
	}

	// Update all children:
	for (auto child_it = this->children.begin();
		 child_it != this->children.end(); ++child_it) {

		GameObject* child = ENGINE->GetSceneGraph()->GetGameObjectById(*child_it);
		if (child != 0) {
			child->Update();
		}
	}
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

void GameObject::AddChild(GameObjectIdType childId) {
	GameObject* child = ENGINE->GetSceneGraph()->GetGameObjectById(childId);
	if (child == 0) {
		// TODO [Cleanup] Assert
		return;
	}

	// Remove old parent:
	GameObject* oldParent = ENGINE->GetSceneGraph()->GetGameObjectById(child->GetParentId());
	if (oldParent != 0) {
		oldParent->removeChild_internal(childId);
	}
	//
	child->removeParent_internal();

	// Set new parent:
	this->addChild_internal(childId);
	//
	child->setParent_internal(this->GetId());
}

// TODO [Cleanup] variable name stds for argument
void GameObject::SetParent(GameObjectIdType newParentId) {
	GameObject* parent = ENGINE->GetSceneGraph()->GetGameObjectById(newParentId);
	if (parent == 0) {
		// TODO [Cleanup] Assert
		return;
	}

	// Remove old parent:
	GameObject* oldParent = ENGINE->GetSceneGraph()->GetGameObjectById(this->parentId);
	if (oldParent != 0) {
		oldParent->removeChild_internal(this->GetId());
	}
	//
	this->removeParent_internal();

	// Set new parent:
	parent->addChild_internal(this->GetId());
	//
	this->setParent_internal(newParentId);
}

// Utility Functions:
void GameObject::addChild_internal(GameObjectIdType childId) {
	this->children.push_back(childId);
}

void GameObject::setParent_internal(GameObjectIdType newParentId) {
	this->parentId = newParentId;
}

void GameObject::removeChild_internal(GameObjectIdType childId) {
	auto child_it = this->findChildById(childId);
	if (child_it != this->children.end()) {
		this->children.erase(child_it);
	}
}

void GameObject::removeParent_internal() {
	this->parentId = GAMEOBJECT_ID_INVALID;
}

std::list<GameObjectIdType>::iterator GameObject::findChildById(GameObjectIdType childId) {
	for (auto child_it = this->children.begin();
		 child_it != this->children.end();
		 ++child_it) {
		if (*child_it == childId) {
			return child_it;
		}
	}

	return this->children.end();
}


void GameObject::destroyAllChildren() {
	while (this->children.begin() != this->children.end()) {
		GameObjectIdType childId = this->children.front();
		GameObject* child = ENGINE->GetSceneGraph()->GetGameObjectById(childId);
		if (child != 0) {
			delete child;
		}
		this->children.pop_front();
	}
}

void GameObject::removeAllComponents() {
	for (auto iter = this->componentMap.begin ();
		 iter != this->componentMap.end (); ++iter) {
		delete iter->second;
	}

	this->componentMap.clear();
}

GameObjectIdType GameObject::getNextFreeId() {
	// TODO [Cleanup] : GameObject Ids begin from 100, for no reason:
	static GameObjectIdType id = 100;
	return id++;
}
