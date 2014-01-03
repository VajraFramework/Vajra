#ifndef CAMERA_H
#define CAMERA_H

#include "Vajra/Common/Components/Component.h"

#include "Libraries/glm/glm.hpp"

// Forward Declarations:
class Object;

enum CameraType_t {
	CAMERA_TYPE_ORTHO,
	CAMERA_TYPE_PERSPECTIVE,
};

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
	void SetCameraType(CameraType_t cameraType_) { this->cameraType = cameraType_; }

	inline glm::mat4& GetViewMatrix() { return this->viewMatrix; }
	inline glm::mat4& GetProjMatrix() { return this->projMatrix; }

	inline float GetFOV() { return this->fov; }
	inline void SetFOV(float value) { this->fov = value; this->updateMatrices();}
private:
	void init();
	void destroy();

	// Utility Functions:
	void updateMatrices();

	CameraType_t cameraType;

	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	// field of view
	float fov;

	static unsigned int componentTypeId;

};

#endif // CAMERA_H
