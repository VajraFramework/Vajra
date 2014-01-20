#ifndef COMPONENT_MAPPER_INTERFACE_H
#define COMPONENT_MAPPER_INTERFACE_H

#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Utilities/Utilities.h"

#include <string>
#include <vector>

class ComponentMapperInterface {
public:
	~ComponentMapperInterface();

	static inline ComponentMapperInterface* GetInstance() {
		ASSERT(ComponentMapperInterface::singleton != nullptr, "ComponentMapperInterface instance has been set");
		return ComponentMapperInterface::singleton;
	}

	virtual Component* AddNewComponentToGameObjectByComponentName(GameObject* gameObject, std::string componentName) = 0;
	virtual void       InitializePropertyByComponentAndPropertyNames(GameObject *gameObject, std::string componentName, std::string propertyName, std::vector<std::string> argv) = 0;

protected:
	ComponentMapperInterface();

private:
	void init();
	void destroy();

	static ComponentMapperInterface* singleton;


};


#endif // COMPONENT_MAPPER_INTERFACE_H
