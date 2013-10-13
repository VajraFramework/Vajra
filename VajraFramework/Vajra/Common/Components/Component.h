#ifndef COMPONENT_H
#define COMPONENT_H

#include "Vajra/Engine/Components/ComponentTypes/ComponentTypeIds.h"
#include "Vajra/Utilities/Utilities.h"

#include <string>
#include <vector>

// Forward Declarations:
class Object;

class Component {
public:
	Component();
	Component(Object* object_);
	virtual ~Component();

	virtual void Update() = 0;

	inline Object* GetObject() { return this->object; }
	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	inline void SetObject(Object* object_) { this->object = object_; }

private:
	void init(Object* object_ = 0);
	void destroy();

	static ComponentIdType componentTypeId;

protected:
	Object* object;
};

#endif // COMPONENT_H
