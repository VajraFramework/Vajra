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
	static void RemoveObject(Object* object);

	static void AddNewObjectByName(Object* object);
	static inline Object* GetObjectByName(std::string name);
	static void RemoveObjectByName(Object* object);

private:
	// Shouldn't need to instantiate this:
	ObjectRegistry();
	ObjectRegistry(const ObjectRegistry&);

	static std::map<int /* id */, Object*> allObjects;
	static std::map<std::string /* name */, Object*> allObjectsByName;
};

////////////////////////////////////////////////////////////////////////////////

// Inline Functions:

Object* ObjectRegistry::GetObjectById(ObjectIdType id) {
	auto object_it = ObjectRegistry::allObjects.find(id);
	if (object_it == ObjectRegistry::allObjects.end()) {
		return nullptr;
	}

	return object_it->second;
}

Object* ObjectRegistry::GetObjectByName(std::string name) {
	auto object_it = ObjectRegistry::allObjectsByName.find(name);
	if (object_it == ObjectRegistry::allObjectsByName.end()) {
		return nullptr;
	}

	return object_it->second;
}


#endif // OBJECT_REPOSITORY_H
