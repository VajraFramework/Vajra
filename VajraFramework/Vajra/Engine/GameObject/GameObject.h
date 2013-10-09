#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vajra/Engine/Components/BaseComponent/Component.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

#include <list>
#include <map>
#include <string>
#include <typeinfo>

// TODO [Cleanup] Move GAMEOBJECT_ID_INVALID somewhere
#define GAMEOBJECT_ID_INVALID 0

// Forward Declarations:
class Component;
class Mesh;
class SceneGraph;
class Transform;


class GameObject {
public:
	GameObject();
	virtual ~GameObject();

	inline int GetId() { return this->id; }
	inline Transform* GetTransform() { return this->transform; }
	inline Mesh* GetMesh() { return this->model; }
	inline int GetParentId() { return this->parentId; }

	virtual void Update();
	virtual void Draw();

	void AddChild(GameObjectIdType childId);
	void SetParent(GameObjectIdType newParentId);

	// Get a Component attached to this GameObject by typename
	TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, T*, Component)
		GetComponent();

	// Create and attach a component to this GameObject. Returns a reference to the new component.
	TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, T*, Component)
		AddComponent();

	// Attach an existing component to this GameObject.
	TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, void, Component)
		AddComponent(T *component);

	// Remove the component with the specified type, if attached.
	TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, void, Component)
		RemoveComponent();


private:
	void init();
	void destroy();

	// Utility Functions:
	void addChild_internal(GameObjectIdType childId);
	void setParent_internal(GameObjectIdType newParentId);
	//
	void removeChild_internal(GameObjectIdType childId);
	void removeParent_internal();
	//
	std::list<GameObjectIdType>::iterator findChildById(GameObjectIdType childId);
	//
	GameObjectIdType getNextFreeId();

	void destroyAllChildren();
	void removeAllComponents();

	GameObjectIdType id;
	std::string name;

	Transform* transform;
	Mesh* model;

	GameObjectIdType parentId;
	// TODO [Implement] Change children from list to map
	std::list<GameObjectIdType /* id */> children;

	std::map<ComponentIdType /* Component id */, Component*> componentMap;

};


// Templated functions:

TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, T*, Component)
GameObject::GetComponent() {
	unsigned int compType = T::GetTypeId();
	auto iter = this->componentMap.find(compType);

	if (iter != componentMap.end ()) {
		return (T*)iter->second;
	}
	return nullptr;
}

TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, T*, Component)
GameObject::AddComponent() {
	unsigned int compType = T::GetTypeId();
	auto iter = this->componentMap.find(compType);
	T* newComponent = new T(this);

	// Don't allow more than one Component with same ID to be attached.
	if (iter != this->componentMap.end()) {
		// Delete the old component at that slot.
		delete iter->second;
		#ifdef DEBUG
			FRAMEWORK->GetLogger()->dbglog("GameObject of id %d, attaching conflicting component of type %s", this->GetId(), typeid(T).name());
		#endif
	}

	this->componentMap[compType] = newComponent;
	// TODO [Implement] Subscribe Components to messages when attaching them:
	// newComponent->SubscribeToMessages();

	return newComponent;
}

TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, void, Component)
GameObject::AddComponent(T *component) {
	unsigned int compType = T::GetTypeId();
	auto iter = this->componentMap.find(compType);

	// Don't allow more than one Component with same ID to be attached.
	if (iter != this->componentMap.end()) {
		// Delete the old component at that slot.
		delete iter->second;
		#ifdef DEBUG
			FRAMEWORK->GetLogger()->dbglog("GameObject of id %d, attaching conflicting component of type %s", this->GetId(), typeid(T).name());
		#endif
	}

	this->componentMap[compType] = component;
	component->SetGameObject(this);
	// TODO [Implement] Subscribe Components to messages when attaching them:
	// component->SubscribeToMessages();
}

TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, void, Component)
GameObject::RemoveComponent() {
	unsigned int compType = T::GetTypeId();
	auto iter = this->componentMap.find(compType);

	if (iter != this->componentMap.end()) {
		delete iter->second;
		this->componentMap.erase(iter);
	}
	#ifdef DEBUG
	else {
		FRAMEWORK->GetLogger()->dbglog("GameObject of id %d, trying to remove unattached Component of type %s", this->GetId(), typeid(T).name());
	}
	#endif

	// TODO [Implement] UnSubscribe Components to messages when detaching them
}

#endif // GAMEOBJECT_H
