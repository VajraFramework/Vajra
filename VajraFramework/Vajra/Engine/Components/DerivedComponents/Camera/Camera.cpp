#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Utilities/MathUtilities.h"

#include "Libraries/glm/gtx/transform.hpp"

unsigned int Camera::componentTypeId = COMPONENT_TYPE_ID_CAMERA;

Camera::Camera() : Component() {
	this->init();
}

Camera::Camera(Object* object_) : Component(object_) {
	this->init();
}

Camera::~Camera() {
	this->destroy();
}

void Camera::updateMatrices() {
	// TODO [Cleanup] This doesn't reflect the state of the transform
	GameObject* gameObject = (GameObject*)this->GetObject();
	glm::vec3 eyePosition = gameObject->GetTransform()->GetPosition();
	// TODO [Implement] Use actual lookAtPosition here
	glm::vec3 lookAtPosition(0.0f, 0.0f, 0.0f);
	glm::vec3 upVector(0.0f, 1.0f, 0.0f);
	if (eyePosition != ZERO_VEC3 && eyePosition != lookAtPosition) {
		this->viewMatrix = glm::lookAt(eyePosition, lookAtPosition, upVector);
	}

    // TODO [Cleanup] 1024 x 768
    this->projMatrix = glm::perspective(60.0f, (float)1024 / (float)768, 0.1f, 8000.0f);
}

void Camera::HandleMessage(Message* message) {
	FRAMEWORK->GetLogger()->dbglog("\nCamera got msg of type %d", message->GetMessageType());

	switch (message->GetMessageType()) {

	case MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT:
		this->updateMatrices();
		break;

	default:
		FRAMEWORK->GetLogger()->dbglog("\nGot unnecessary msg of type %d", message->GetMessageType());
	}
}

void Camera::WriteLookAt() {
	// TODO [Implement] Camera::WriteLookAt()
}

void Camera::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->viewMatrix = IDENTITY_MATRIX;
	this->projMatrix = IDENTITY_MATRIX;

	if (gameObject != nullptr) {
		this->updateMatrices();
	}

	this->addSubscriptionToMessageType(MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT, this->GetTypeId(), true);
}

void Camera::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

