#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

std::map<int /* id */, Object*> ObjectRegistry::allObjects;

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

ObjectRegistry::ObjectRegistry() {
	VERIFY(0, "ObjectRegistry shouldn't be instantiated");
}

ObjectRegistry::ObjectRegistry(const ObjectRegistry&) {
	VERIFY(0, "ObjectRegistry shouldn't be instantiated");
}
