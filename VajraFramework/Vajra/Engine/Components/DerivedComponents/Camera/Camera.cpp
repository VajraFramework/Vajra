#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"
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
	GameObject* gameObject = (GameObject*)this->GetObject();
	//
	glm::vec3 eyePosition    = gameObject->GetTransform()->GetPosition();
	glm::vec3 lookAtPosition = eyePosition + 10.0f * gameObject->GetTransform()->GetForward();
	glm::vec3 upVector       = gameObject->GetTransform()->GetUp();
	//
	this->viewMatrix = glm::lookAt(eyePosition, lookAtPosition, upVector);

	float width  = FRAMEWORK->GetDeviceProperties()->GetWidthPixels();
	float height = FRAMEWORK->GetDeviceProperties()->GetHeightPixels();

	// TODO [Hack] : remove once device tilting is suported
#if 0
	float aspecRatio = width / height;
#else
	float aspecRatio = height / width;
#endif
	
	switch (this->cameraType) {

	case CAMERA_TYPE_ORTHO: {
			this->projMatrix = glm::ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f, 0.1f, 8000.0f);
		} break;

	case CAMERA_TYPE_PERSPECTIVE: {
			this->projMatrix = glm::perspective(this->fov, aspecRatio, 0.1f, 8000.0f);
		} break;

	default: {
			ASSERT(0, "Unknown camera type, %d", this->cameraType);
		} break;
	}
}

void Camera::HandleMessage(Message* message) {
	// FRAMEWORK->GetLogger()->dbglog("\nCamera got msg of type %d", message->GetMessageType());

	switch (message->GetMessageType()) {

	case MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT:
		this->updateMatrices();
		break;

	default:
		FRAMEWORK->GetLogger()->dbglog("\nCamera got unnecessary msg of type %d", message->GetMessageType());
		break;
	}
}

void Camera::WriteLookAt() {
	// TODO [Implement] Camera::WriteLookAt(), will be needed when the shader starts looking for the eyePosition (example: for specular lighting)
}

void Camera::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->cameraType = CAMERA_TYPE_PERSPECTIVE;

	this->viewMatrix = IDENTITY_MATRIX;
	this->projMatrix = IDENTITY_MATRIX;

	this->fov = 60.0f;
	if (gameObject != nullptr) {
		this->updateMatrices();
	}

	this->addSubscriptionToMessageType(MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT, this->GetTypeId(), true);
}

void Camera::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

