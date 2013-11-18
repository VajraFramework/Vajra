#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vajra/Common/Components/Component.h"

#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/quaternion.hpp"
#include "Libraries/glm/gtx/quaternion.hpp"

// Forward Declarations:
class GameObject;

class Transform : public Component {
public:
	Transform();
	Transform(Object* object_);
	~Transform();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(Message* message);

	void Draw();

	glm::vec3& GetPosition();
	glm::quat& GetOrientation();
	glm::vec3& GetScale();

	void SetPosition(float x, float y, float z);
	void SetPosition(glm::vec3 newPosition);
	void SetOrientation(float angleInDegrees, float x, float y, float z);
	void SetOrientation(float angleInDegrees, glm::vec3 axis);
	void SetOrientation(glm::quat newOrientation);
	void SetScale(float x, float y, float z);
	//
	void Translate(glm::vec3 diff);
	void Translate(float distance, glm::vec3 along);
	void Rotate(float angleInDegrees, glm::vec3 axis);
	void Rotate(glm::quat quaternion);
	void Scale(float x, float y, float z);
	//
	void LookAt(float point_x, float point_y, float point_z);
	void LookAt(glm::vec3 point);

	glm::vec3& GetForward() { return this->forward; }
	glm::vec3& GetLeft() { return this->left; }
	glm::vec3& GetUp() { return this->up; }

private:
	void init();
	void destroy();

	// Utility Transform Functions:
	void setPosition(glm::vec3 newPosition);
	void setOrientation(glm::quat newOrientation);
	void setScale(glm::vec3 newScale);

	void updateModelMatrix();
	void updateModelMatrixCumulative();
	void rippleMatrixUpdates();

	static unsigned int componentTypeId;

	glm::vec3 position;			/// Position of this GameObject in its Parent's co-ordinate space
	glm::quat orientation;		/// Rotation (a Quaternion) of this GameObject in its Paren't co-ordinate space
	glm::vec3 scale;			/// Scale of this GameObject in its Parent's co-ordinate space

	glm::vec3 forward;
	glm::vec3 left;
	glm::vec3 up;

	glm::mat4 modelMatrix;
	glm::mat4 modelMatrixCumulative;

};

#endif // TRANSFORM_H
