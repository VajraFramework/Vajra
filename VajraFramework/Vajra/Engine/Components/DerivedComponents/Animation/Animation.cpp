#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/Animation.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/MathUtilities.h"

unsigned int Animation::componentTypeId = COMPONENT_TYPE_ID_ANIMATION;

Animation::Animation() : Component() {
	this->init();
}

Animation::Animation(Object* object_) : Component(object_) {
	this->init();
}

Animation::~Animation() {
	this->destroy();
}

void Animation::HandleMessage(Message* message) {
	switch (message->GetMessageType()) {

	default:
		break;
	}
}

void Animation::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

}

void Animation::destroy() {
}

