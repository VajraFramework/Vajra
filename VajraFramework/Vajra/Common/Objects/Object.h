#ifndef OBJECT_H
#define OBJECT_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Common/Messages/Declarations.h"
#include "Vajra/Common/Objects/Declarations.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

#include <list>
#include <map>
#include <string>
#include <typeinfo>

// Forward Declarations:
class Component;

class Object {
public:
	Object();
	virtual ~Object();

	inline int GetId() { return this->id; }
	inline int GetParentId() { return this->parentId; }
	inline std::string GetName() { return this->name; }

	inline const std::list<ObjectIdType>& GetChildren() { return this->children; }

	void AddChild(ObjectIdType childId);
	void SetParent(ObjectIdType newParentId);

	void HandleMessages();

	void SubscribeToMessageType(MessageType messageType, ComponentIdType subscriberComponentId, bool onLocalObject);
	void UnsubscribeToMessageType(MessageType messageType, ComponentIdType subscriberComponentId);

	// Get a Component attached to this Object by typename
	TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, T*, Component)
		GetComponent();

	// Create and attach a component to this Object. Returns a reference to the new component.
	TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, T*, Component)
		AddComponent();

	// Attach an existing component to this Object.
	TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, void, Component)
		AddComponent(T *component);

	// Remove the component with the specified type, if attached.
	TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, void, Component)
		RemoveComponent();

protected:
	// TODO [Implement] Change children from list to map, maybe
	std::list<ObjectIdType /* id */> children;

	std::map<ComponentIdType /* Component id */, Component*> componentMap;

private:
	void init();
	void destroy();

	// Utility Functions:
	void addChild_internal(ObjectIdType childId);
	void setParent_internal(ObjectIdType newParentId);
	//
	void removeChild_internal(ObjectIdType childId);
	void removeParent_internal();
	//
	std::list<ObjectIdType>::iterator findChildById(ObjectIdType childId);
	//
	ObjectIdType getNextFreeId();

	void destroyAllChildren();
	void removeAllComponents();

	ObjectIdType id;
	std::string name;

	ObjectIdType parentId;

	std::vector<std::vector<ComponentIdType>> subscribersForMessageType;

friend class SceneGraph;
};


// Templated functions:

TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, T*, Component)
Object::GetComponent() {
	unsigned int compType = T::GetTypeId();
	auto iter = this->componentMap.find(compType);

	if (iter != componentMap.end ()) {
		return (T*)iter->second;
	}
	return nullptr;
}

TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, T*, Component)
Object::AddComponent() {
	unsigned int compType = T::GetTypeId();
	auto iter = this->componentMap.find(compType);
	T* newComponent = new T(this);

	// Don't allow more than one Component with same ID to be attached.
	if (iter != this->componentMap.end()) {
		// Delete the old component at that slot.
		delete iter->second;
		#ifdef DEBUG
			FRAMEWORK->GetLogger()->dbglog("Object of id %d, attaching conflicting component of type %s", this->GetId(), typeid(T).name());
		#endif
	}

	this->componentMap[compType] = newComponent;
	// TODO [Implement] Subscribe Components to messages when attaching them:
	// newComponent->SubscribeToMessages();

	return newComponent;
}

TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, void, Component)
Object::AddComponent(T *component) {
	unsigned int compType = T::GetTypeId();
	auto iter = this->componentMap.find(compType);

	// Don't allow more than one Component with same ID to be attached.
	if (iter != this->componentMap.end()) {
		// Delete the old component at that slot.
		delete iter->second;
		#ifdef DEBUG
			FRAMEWORK->GetLogger()->dbglog("Object of id %d, attaching conflicting component of type %s", this->GetId(), typeid(T).name());
		#endif
	}

	this->componentMap[compType] = component;
	component->SetObject(this);
	// TODO [Implement] Subscribe Components to messages when attaching them:
	// component->SubscribeToMessages();
}

TEMPLATED_RETURNTYPE_IF_IS_BASE_OF(T, void, Component)
Object::RemoveComponent() {
	unsigned int compType = T::GetTypeId();
	auto iter = this->componentMap.find(compType);

	if (iter != this->componentMap.end()) {
		delete iter->second;
		this->componentMap.erase(iter);
	}
	#ifdef DEBUG
	else {
		FRAMEWORK->GetLogger()->dbglog("Object of id %d, trying to remove unattached Component of type %s", this->GetId(), typeid(T).name());
	}
	#endif

	// TODO [Implement] UnSubscribe Components to messages when detaching them
}

#endif // OBJECT_H
