#include "Vajra/Engine/Prefabs/ComponentMapperInterface/ComponentMapperInterface.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"

ComponentMapperInterface* ComponentMapperInterface::singleton = nullptr;

ComponentMapperInterface::ComponentMapperInterface() {
	this->init();
}

ComponentMapperInterface::~ComponentMapperInterface() {
	this->destroy();
}

void ComponentMapperInterface::init() {
	ASSERT(ComponentMapperInterface::singleton == nullptr, "ComponentMapperInterface instance created only once");
	ComponentMapperInterface::singleton = this;
}

void ComponentMapperInterface::destroy() {
}
