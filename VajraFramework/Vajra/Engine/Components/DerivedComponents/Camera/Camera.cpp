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

Camera::Camera(GameObject* gameObject_) : Component(gameObject_) {
	this->init();
}

Camera::~Camera() {
	this->destroy();
}

void Camera::Update() {
	// TODO [Cleanup] This doesn't need to be done every frame: cleanup to use messages instead, maybe
	glm::vec3 eyePosition = this->gameObject->GetTransform()->GetPosition();
	// TODO [Implement] Use actual lookAtPosition here
	glm::vec3 lookAtPosition(0.0f, 0.0f, 0.0f);
	glm::vec3 upVector(0.0f, 1.0f, 0.0f);
    this->viewMatrix = glm::lookAt(eyePosition, lookAtPosition, upVector);

    // TODO [Cleanup] 1024 x 768
    this->projMatrix = glm::perspective(60.0f, (float)1024 / (float)768, 0.1f, 8000.0f);
}

void Camera::WriteLookAt() {
	// TODO [Implement] Camera::WriteLookAt()
}

void Camera::init() {
	this->viewMatrix = IDENTITY_MATRIX;
	this->projMatrix = IDENTITY_MATRIX;
}

void Camera::destroy() {
}
