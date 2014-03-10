#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Framework/DeviceUtils/DeviceProperties/DeviceProperties.h"

#include "Libraries/glm/gtx/transform.hpp"

unsigned int Camera::componentTypeId = COMPONENT_TYPE_ID_CAMERA;

/*
 * Frustum Culling Reference:  http://web.archive.org/web/20120531231005/http://crazyjoke.free.fr/doc/3D/plane%20extraction.pdf
 */

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
			this->projMatrix = glm::ortho(this->ortho_bounds_x_min, this->ortho_bounds_x_max,
										  this->ortho_bounds_y_min, this->ortho_bounds_y_max,
										  this->ortho_bounds_z_min, this->ortho_bounds_z_max);
		} break;

	case CAMERA_TYPE_PERSPECTIVE: {
			this->projMatrix = glm::perspective(this->fov, aspecRatio, 0.3f, 100.0f);
		} break;

	default: {
			ASSERT(0, "Unknown camera type, %d", this->cameraType);
		} break;
	}

	this->updateFrustum();
}

void Camera::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);
	// FRAMEWORK->GetLogger()->dbglog("\nCamera got msg of type %d", messageChunk->GetMessageType());

	switch (messageChunk->GetMessageType()) {

	case MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT:
		this->updateMatrices();
		break;

	}
}

void Camera::SetOrthoBounds(float x_min, float x_max, float y_min, float y_max, float z_min, float z_max) {
	this->ortho_bounds_x_min = x_min;
	this->ortho_bounds_x_max = x_max;
	this->ortho_bounds_y_min = y_min;
	this->ortho_bounds_y_max = y_max;
	this->ortho_bounds_z_min = z_min;
	this->ortho_bounds_z_max = z_max;

	this->updateMatrices();
}

void Camera::SetCameraType(CameraType_t cameraType_) {
	this->cameraType = cameraType_;

	this->updateMatrices();
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

void Camera::updateFrustum() {
	glm::mat4 viewProj = this->projMatrix * this->viewMatrix;


	// Left plane
	this->frustumPlanes[PLANE_LEFT] = glm::vec4(
								viewProj[0][3] + viewProj[0][0] ,
								viewProj[1][3] + viewProj[1][0] ,
								viewProj[2][3] + viewProj[2][0] ,
								viewProj[3][3] + viewProj[3][0] );

	// Right plane
	this->frustumPlanes[PLANE_RIGHT] = glm::vec4(
								viewProj[0][3] - viewProj[0][0] ,
								viewProj[1][3] - viewProj[1][0] ,
								viewProj[2][3] - viewProj[2][0] ,
								viewProj[3][3] - viewProj[3][0] );


    	// Top plane
	this->frustumPlanes[PLANE_TOP] = glm::vec4(
								viewProj[0][3] - viewProj[0][1] ,
								viewProj[1][3] - viewProj[1][1] ,
								viewProj[2][3] - viewProj[2][1] ,
								viewProj[3][3] - viewProj[3][1] );

    	// Bottom plane
	this->frustumPlanes[PLANE_BOTTOM] = glm::vec4(
								viewProj[0][3] + viewProj[0][1] ,
								viewProj[1][3] + viewProj[1][1] ,
								viewProj[2][3] + viewProj[2][1] ,
								viewProj[3][3] + viewProj[3][1] );

    	// Near plane
	this->frustumPlanes[PLANE_NEAR] = glm::vec4(
								viewProj[0][3] + viewProj[0][2] ,
								viewProj[1][3] + viewProj[1][2] ,
								viewProj[2][3] + viewProj[2][2] ,
								viewProj[3][3] + viewProj[3][2] );

    	// Far plane
	this->frustumPlanes[PLANE_FAR] = glm::vec4(
								viewProj[0][3] - viewProj[0][2] ,
								viewProj[1][3] - viewProj[1][2] ,
								viewProj[2][3] - viewProj[2][2] ,
								viewProj[3][3] - viewProj[3][2] );




	for (int i = 0 ; i < PLANE_NUM_PLANES; ++i) {
		float magnitude = sqrt (this->frustumPlanes[i].x * this->frustumPlanes[i].x +
								this->frustumPlanes[i].y * this->frustumPlanes[i].y +
								this->frustumPlanes[i].z * this->frustumPlanes[i].z);
		this->frustumPlanes[i][0] /= magnitude;
		this->frustumPlanes[i][1] /= magnitude;
		this->frustumPlanes[i][2] /= magnitude;
		this->frustumPlanes[i][3] /= magnitude;
	}
}


bool Camera::IsPointInFrustum(glm::vec3 point, float toleranceRadius /* = 0.0f */) {

	// TODO [Hack] Don't check against near and far planes, save computation
	for (int i = 0 ; i < PLANE_NEAR; ++i) {
		float signed_distance = DistanceFromPlaneToPoint(this->frustumPlanes[i], point);
		if (!((signed_distance + toleranceRadius) >= 0 ||
			  (signed_distance - toleranceRadius) >= 0)) {
			return false;
		}
	}

	return true;
}

void Camera::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->cameraType = CAMERA_TYPE_PERSPECTIVE;

	this->viewMatrix = IDENTITY_MATRIX;
	this->projMatrix = IDENTITY_MATRIX;

	this->fov = 60.0f inRadians;

	float width  = FRAMEWORK->GetDeviceProperties()->GetWidthPixels();
	float height = FRAMEWORK->GetDeviceProperties()->GetHeightPixels();
	// Sane default values:
	this->ortho_bounds_x_min = 0.0f;
	this->ortho_bounds_x_max = width;
	this->ortho_bounds_y_min = -height;
	this->ortho_bounds_y_max = 0.0f;
	this->ortho_bounds_z_min = -500.0f;
	this->ortho_bounds_z_max = 500.0f;

	if (gameObject != nullptr) {
		this->updateMatrices();
	}

	this->addSubscriptionToMessageType(MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT, this->GetTypeId(), true);
}

void Camera::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

