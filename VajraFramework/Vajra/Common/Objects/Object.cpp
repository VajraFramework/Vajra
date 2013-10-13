#include "Vajra/Common/Components/Component.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

Object::Object() {
	this->init();
}

Object::~Object() {
	this->destroy();
}

void Object::init() {
	this->id = this->getNextFreeId();
	ObjectRegistry::AddNewObject(this);

	this->parentId = OBJECT_ID_INVALID;

	FRAMEWORK->GetLogger()->dbglog("\nCreated new Object of id: %d", this->GetId());
}

void Object::destroy() {
	this->destroyAllChildren();
	this->removeAllComponents();
}

void Object::HandleMessages() {
	Message* message = nullptr;
	do {
		message = ENGINE->GetMessageHub()->RetrieveNextMessage(this->GetId());
		if (message != nullptr) {
			FRAMEWORK->GetLogger()->dbglog("\nGot msg of type %d", message->GetMessageType());
		}
	} while (message != nullptr);
}

void Object::AddChild(ObjectIdType childId) {
	Object* child = ObjectRegistry::GetObjectById(childId);
	if (child == 0) {
		// TODO [Cleanup] Assert
		return;
	}

	// Remove old parent:
	Object* oldParent = ObjectRegistry::GetObjectById(child->GetParentId());
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
void Object::SetParent(ObjectIdType newParentId) {
	Object* parent = ObjectRegistry::GetObjectById(newParentId);
	if (parent == 0) {
		// TODO [Cleanup] Assert
		return;
	}

	// Remove old parent:
	Object* oldParent = ObjectRegistry::GetObjectById(this->parentId);
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
void Object::addChild_internal(ObjectIdType childId) {
	this->children.push_back(childId);
}

void Object::setParent_internal(ObjectIdType newParentId) {
	this->parentId = newParentId;
}

void Object::removeChild_internal(ObjectIdType childId) {
	auto child_it = this->findChildById(childId);
	if (child_it != this->children.end()) {
		this->children.erase(child_it);
	}
}

void Object::removeParent_internal() {
	this->parentId = OBJECT_ID_INVALID;
}

std::list<ObjectIdType>::iterator Object::findChildById(ObjectIdType childId) {
	for (auto child_it = this->children.begin();
		 child_it != this->children.end();
		 ++child_it) {
		if (*child_it == childId) {
			return child_it;
		}
	}

	return this->children.end();
}


void Object::destroyAllChildren() {
	while (this->children.begin() != this->children.end()) {
		ObjectIdType childId = this->children.front();
		Object* child = ObjectRegistry::GetObjectById(childId);
		if (child != 0) {
			delete child;
		}
		this->children.pop_front();
	}
}

void Object::removeAllComponents() {
	for (auto iter = this->componentMap.begin ();
		 iter != this->componentMap.end (); ++iter) {
		delete iter->second;
	}

	this->componentMap.clear();
}

ObjectIdType Object::getNextFreeId() {
	// TODO [Cleanup] : Object Ids begin from 100, for no reason:
	static ObjectIdType id = 100;
	return id++;
}
