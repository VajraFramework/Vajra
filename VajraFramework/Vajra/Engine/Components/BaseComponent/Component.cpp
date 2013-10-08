#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Components/BaseComponent/Component.h"
#include "Vajra/Framework/Logging/Logger.h"

ComponentIdType Component::componentTypeId = COMPONENT_TYPE_ID_DEFAULT;

Component::Component() {
	this->init();
}

Component::Component(GameObject* gameObject_) {
	this->init(gameObject_);
}

Component::~Component() {
	this->destroy();
}

void Component::init(GameObject* gameObject_ /* = 0 */) {
	this->gameObject = gameObject_;
}

void Component::destroy() {
}
