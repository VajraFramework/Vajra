#ifndef CAMERA_H
#define CAMERA_H

#include "Vajra/Engine/Components/BaseComponent/Component.h"

#include "Libraries/glm/glm.hpp"

// Forward Declarations:
class GameObject;

class Camera : public Component {
public:
	Camera();
	Camera(GameObject* gameObject_);
	~Camera();

	static inline unsigned int GetTypeId() { return componentTypeId; }

	// @Override
	virtual void Update();

	void WriteLookAt();

	inline glm::mat4& GetViewMatrix() { return this->viewMatrix; }
	inline glm::mat4& GetProjMatrix() { return this->projMatrix; }

private:
	void init();
	void destroy();

	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	static unsigned int componentTypeId;

};

#endif // CAMERA_H
