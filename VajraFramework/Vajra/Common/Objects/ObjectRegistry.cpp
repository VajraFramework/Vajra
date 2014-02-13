#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

std::map<int /* id */, Object*> ObjectRegistry::allObjects;
std::map<std::string /* name */, Object*> ObjectRegistry::allObjectsByName;

void ObjectRegistry::AddNewObject(Object* object) {
	VERIFY(object->GetId() > OBJECT_ID_INVALID,
		"Object has valid id: %d", object->GetId());
	VERIFY(ObjectRegistry::GetObjectById(object->GetId()) == nullptr,
		"Object has unique id: %d", object->GetId());

	FRAMEWORK->GetLogger()->dbglog("\nAdding Object with id %d to ObjectRegistry", object->GetId());

	// Add this Object to the map of all Objects:
	ObjectRegistry::allObjects[object->GetId()] = object;
}

void ObjectRegistry::RemoveObject(Object* object) {
	VERIFY(object->GetId() > OBJECT_ID_INVALID,
		"Object has valid id: %d", object->GetId());
	VERIFY(ObjectRegistry::GetObjectById(object->GetId()) != nullptr,
		"Object of id %d exists in the object registry", object->GetId());

	FRAMEWORK->GetLogger()->dbglog("\nRemoving Object with id %d from ObjectRegistry", object->GetId());

	// Remove this Object from the map of all Objects:
	int returnValueOfErasing = ObjectRegistry::allObjects.erase(object->GetId());
	VERIFY(returnValueOfErasing == 1, "Erasing object from map returned %d, should have returned 1", returnValueOfErasing);
}

void ObjectRegistry::AddNewObjectByName(Object* object) {
	VERIFY(object->GetName() != "",
		"Object has valid name");
	VERIFY(ObjectRegistry::GetObjectByName(object->GetName()) == nullptr,
		"Object has unique name: %s", object->GetName().c_str());

	FRAMEWORK->GetLogger()->dbglog("\nAdding Object with name %s to ObjectRegistry", object->GetName().c_str());

	// Add this Object to the map of all Objects:
	ObjectRegistry::allObjectsByName[object->GetName()] = object;
}

void ObjectRegistry::RemoveObjectByName(Object* object) {
	VERIFY(object->GetName() != "",
		"Object has valid name");
	VERIFY(ObjectRegistry::GetObjectByName(object->GetName()) != nullptr,
		"Object of name %s exists in the object registry", object->GetName().c_str());

	FRAMEWORK->GetLogger()->dbglog("\nRemoving Object with name %s from ObjectRegistry", object->GetName().c_str());

	// Remove this Object from the map of all Objects:
	int returnValueOfErasing = ObjectRegistry::allObjectsByName.erase(object->GetName());
	VERIFY(returnValueOfErasing == 1, "Erasing object from map returned %d, should have returned 1", returnValueOfErasing);
}

ObjectRegistry::ObjectRegistry() {
	VERIFY(0, "ObjectRegistry shouldn't be instantiated");
}

ObjectRegistry::ObjectRegistry(const ObjectRegistry&) {
	VERIFY(0, "ObjectRegistry shouldn't be instantiated");
}
