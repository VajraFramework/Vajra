#ifndef CAMERA_H
#define CAMERA_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Utilities/MathUtilities.h"

#include "Libraries/glm/glm.hpp"

// Forward Declarations:
class Object;

enum CameraType_t {
	CAMERA_TYPE_ORTHO,
	CAMERA_TYPE_PERSPECTIVE,
};

enum PlaneType_t {
	PLANE_LEFT = 0,
	PLANE_RIGHT,
	PLANE_BOTTOM,
	PLANE_TOP,
	PLANE_NEAR,
	PLANE_FAR,
	PLANE_NUM_PLANES
};

//[[COMPONENT]]//
class Camera : public Component {
public:
	Camera();
	Camera(Object* object_);
	~Camera();

	static inline unsigned int GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	void WriteLookAt();

	CameraType_t GetCameraType() { return this->cameraType; }
	//[[PROPERTY]]//
	inline void SetCameraType(int cameraTypeInt);
	void SetCameraType(CameraType_t cameraType_) { this->cameraType = cameraType_; }

	inline glm::mat4& GetViewMatrix() { return this->viewMatrix; }
	inline glm::mat4& GetProjMatrix() { return this->projMatrix; }

	inline float GetFOV();
	//[[PROPERTY]]//
	inline void SetFOV(float value);

	glm::vec3 ScreenToWorldPoint(glm::vec3 screenPoint);
	Ray ScreenPointToRay(glm::vec2 screenPoint);

	bool IsPointInFrustum(glm::vec3 point);

private:
	void init();
	void destroy();

	// Utility Functions:
	void updateMatrices();
	void updateFrustum();

	CameraType_t cameraType;

	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	glm::vec4 frustumPlanes[PLANE_NUM_PLANES];

	// field of view
	float fov;

	static unsigned int componentTypeId;

};

////////////////////////////////////////////////////////////////////////////////

// Inline Functions:

inline float Camera::GetFOV() {
	return this->fov;
}

void Camera::SetFOV(float value) {
	this->fov = value;
	this->updateMatrices();
}

void Camera::SetCameraType(int cameraTypeInt) {
	this->SetCameraType(cameraTypeInt);
}

#endif // CAMERA_H
