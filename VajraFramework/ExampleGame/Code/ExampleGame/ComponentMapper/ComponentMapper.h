#ifndef COMPONENT_MAPPER
#define COMPONENT_MAPPER

#include "Vajra/Engine/Prefabs/ComponentMapperInterface/ComponentMapperInterface.h"

class ComponentMapper : public ComponentMapperInterface {
public:
	ComponentMapper()  {}
	~ComponentMapper() {}

	// @Override
	virtual Component* AddNewComponentToGameObjectByComponentName(GameObject* gameObject, std::string componentName);
	virtual void       InitializePropertyByComponentAndPropertyNames(GameObject *gameObject, std::string componentName, std::string propertyName, std::vector<std::string> argv);

private:
};

#endif // COMPONENT_MAPPER
