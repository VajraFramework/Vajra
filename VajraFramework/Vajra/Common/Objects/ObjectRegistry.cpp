#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/Utilities.h"

std::map<int /* id */, Object*> ObjectRegistry::allObjects;

void ObjectRegistry::AddNewObject(Object* object) {
	ASSERT(object->GetId() > OBJECT_ID_INVALID,
		"Object has valid id: %d", object->GetId());
	ASSERT(ObjectRegistry::GetObjectById(object->GetId()) == nullptr,
		"Object has unique id: %d", object->GetId());

	FRAMEWORK->GetLogger()->dbglog("\nAdding Object with id %d to ObjectRegistry", object->GetId());

	// Add this Object to the map of all Objects:
	ObjectRegistry::allObjects[object->GetId()] = object;
}

ObjectRegistry::ObjectRegistry() {
	ASSERT(0, "ObjectRegistry shouldn't be instantiated");
}

ObjectRegistry::ObjectRegistry(const ObjectRegistry&) {
	ASSERT(0, "ObjectRegistry shouldn't be instantiated");
}
