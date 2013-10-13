#include "Vajra/Common/Components/Component.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Framework/Logging/Logger.h"

ComponentIdType Component::componentTypeId = COMPONENT_TYPE_ID_DEFAULT;

Component::Component() {
	this->init();
}

Component::Component(Object* object_) {
	this->init(object_);
}

Component::~Component() {
	this->destroy();
}

void Component::init(Object* object_ /* = 0 */) {
	this->object = object_;
}

void Component::destroy() {
}
