#ifndef COMPONENT_H
#define COMPONENT_H

#include "Vajra/Engine/Components/ComponentTypes/ComponentTypeIds.h"
#include "Vajra/Utilities/Utilities.h"

#include <string>
#include <vector>

// Forward Declarations:
class GameObject;

class Component {
public:
	Component();
	Component(GameObject* gameObject_);
	virtual ~Component();

	virtual void Update() = 0;

	inline GameObject* GetGameObject() { return this->gameObject; }
	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	inline void SetGameObject(GameObject* gameObject_) { this->gameObject = gameObject_; }

private:
	void init(GameObject* gameObject_ = 0);
	void destroy();


	static ComponentIdType componentTypeId;

protected:
	GameObject* gameObject;
};

#endif // COMPONENT_H
