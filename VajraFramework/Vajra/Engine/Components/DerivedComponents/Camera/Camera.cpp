#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"

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

	float aspecRatio = width / height;
	
	switch (this->cameraType) {

	case CAMERA_TYPE_ORTHO: {
			this->projMatrix = glm::ortho(0.0f, width, -height, 0.0f, 0.1f, 8000.0f);
		} break;

	case CAMERA_TYPE_PERSPECTIVE: {
			this->projMatrix = glm::perspective(this->fov, aspecRatio, 0.1f, 8000.0f);
		} break;

	default: {
			ASSERT(0, "Unknown camera type, %d", this->cameraType);
		} break;
	}
}

void Camera::HandleMessage(MessageChunk messageChunk) {
	// FRAMEWORK->GetLogger()->dbglog("\nCamera got msg of type %d", messageChunk->GetMessageType());

	switch (messageChunk->GetMessageType()) {

	case MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT:
		this->updateMatrices();
		break;

	}
}

void Camera::WriteLookAt() {
	// TODO [Implement] Camera::WriteLookAt(), will be needed when the shader starts looking for the eyePosition (example: for specular lighting)
}

glm::vec3 Camera::ScreenToWorldPoint(glm::vec3 screenPoint) {
	float screenW = FRAMEWORK->GetDeviceProperties()->GetWidthPixels();
    float screenH = FRAMEWORK->GetDeviceProperties()->GetHeightPixels();
    glm::vec4 viewport = glm::vec4(0.0f, 0.0f, screenW, screenH);
    glm::mat4 tmpView = this->viewMatrix;
    glm::mat4 tmpProj = this->projMatrix;
    glm::vec3 screenPos = glm::vec3(screenPoint.x, screenH - screenPoint.y, screenPoint.z);
    glm::vec3 worldPos = glm::unProject(screenPos, tmpView, tmpProj, viewport);
#if 0
	GameObject* gameObject = (GameObject*)this->GetObject();
	glm::vec3 eyePosition    = gameObject->GetTransform()->GetPosition();
	FRAMEWORK->GetLogger()->dbglog("\ncam pos: %f, %f, %f", eyePosition.x, eyePosition.y, eyePosition.z);
	FRAMEWORK->GetLogger()->dbglog("\nviewport: %f, %f", viewport.z, viewport.w);
	FRAMEWORK->GetLogger()->dbglog("\nworld pos: %f, %f, %f", worldPos.x, worldPos.y, worldPos.z);
#endif
	return worldPos;
}

Ray Camera::ScreenPointToRay(glm::vec2 screenPoint) {
	Ray screenRay;
	glm::vec3 nearPlanePoint = this->ScreenToWorldPoint(glm::vec3(screenPoint.x, screenPoint.y, -1.0f));
	glm::vec3 farPlanePoint = this->ScreenToWorldPoint(glm::vec3(screenPoint.x, screenPoint.y, 1.0f));
	glm::vec3 dir = farPlanePoint - nearPlanePoint;
	dir = glm::normalize(dir);
	screenRay.origin = nearPlanePoint;
	screenRay.dir = dir;
#if 0
	FRAMEWORK->GetLogger()->dbglog("\ray dir: %f, %f, %f", dir.x, dir.y, dir.z);
#endif
	return screenRay;
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

