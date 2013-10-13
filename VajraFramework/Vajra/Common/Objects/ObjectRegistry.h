#ifndef OBJECT_REPOSITORY_H
#define OBJECT_REPOSITORY_H

#include "Vajra/Utilities/CommonDeclarations.h"

#include <map>

// Forward Declarations:
class Object;

class ObjectRegistry {
public:
	static void AddNewObject(Object* object);
	static inline Object* GetObjectById(ObjectIdType id);

private:
	// Shouldn't need to instantiate this:
	ObjectRegistry();
	ObjectRegistry(const ObjectRegistry&);

	static std::map<int /* id */, Object*> allObjects;
};

// Inline Functions:

Object* ObjectRegistry::GetObjectById(ObjectIdType id) {
	auto object_it = ObjectRegistry::allObjects.find(id);
	if (object_it == ObjectRegistry::allObjects.end()) {
		return nullptr;
	}

	return object_it->second;
}


#endif // OBJECT_REPOSITORY_H
