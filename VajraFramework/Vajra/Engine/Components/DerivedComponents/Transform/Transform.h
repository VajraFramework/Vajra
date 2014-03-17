#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Declarations.h"

#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/quaternion.hpp"
#include "Libraries/glm/gtx/quaternion.hpp"

// Forward Declarations:
class GameObject;

//[[COMPONENT]]//
class Transform : public Component {
public:
	Transform();
	Transform(Object* object_);
	~Transform();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	void Draw();

	glm::vec3& GetPosition();
	glm::quat& GetOrientation();
	glm::vec3& GetScale();
	//
	glm::vec3& GetPositionWorld();
	glm::quat& GetOrientationWorld();
	glm::vec3& GetScaleWorld();

	//[[PROPERTY]]//
	void SetPosition(float x, float y, float z);
	void SetPosition(glm::vec3 newPosition);
	void SetPositionWorld(float x, float y, float z);
	void SetPositionWorld(glm::vec3 newPosition);
	//[[PROPERTY]]//
	void SetOrientation(float angleInRadians, float x, float y, float z);
	void SetOrientation(float angleInRadians, glm::vec3 axis);
	void SetOrientation(glm::quat newOrientation);
	//[[PROPERTY]]//
	void SetScale(float x, float y, float z);
	void SetScale(glm::vec3 newScale);
	//
	//[[PROPERTY]]//
	void Translate(float x, float y, float z);
	void Translate(glm::vec3 diff);
	void Translate(float distance, glm::vec3 along);
	//[[PROPERTY]]//
	void Rotate(float angleInRadians, float x, float y, float z);
	void Rotate(float angleInRadians, glm::vec3 axis);
	void Rotate(glm::quat quaternion);
	//[[PROPERTY]]//
	void RotateByDegrees(float angleInDegrees, float x, float y, float z);
	void Scale(float x, float y, float z);
	void Scale(glm::vec3 scaleVector);
	//[[PROPERTY]]//
	void Scale(float scaleFactor);
	//
	//[[PROPERTY]]//
	void LookAt(float point_x, float point_y, float point_z);
	void LookAt(glm::vec3 point);

	// TODO [Hack] This shouldn't really be used. Guard this better, maybe?
	void SetModelMatrixCumulative(glm::mat4 requiredModelMatrixCumulative);
	glm::mat4 GetModelMatrixCumulative() { return this->modelMatrixCumulative; }

	glm::vec3& GetForward() { return this->forward; }
	glm::vec3& GetLeft() { return this->left; }
	glm::vec3& GetUp() { return this->up; }

private:
	void init();
	void destroy();

	// Utility Transform Functions:
	void setPosition(glm::vec3 newPosition);
	void setPositionWorld(glm::vec3 newPosition);
	void setOrientation(glm::quat newOrientation);
	void setScale(glm::vec3 newScale);

	void updateModelMatrix();
	void updateModelMatrixCumulative();
	void rippleMatrixUpdates();

	static unsigned int componentTypeId;

	glm::vec3 position;			/// Position of this GameObject in its Parent's co-ordinate space
	glm::quat orientation;		/// Rotation (a Quaternion) of this GameObject in its Parent's co-ordinate space
	glm::vec3 scale;			/// Scale of this GameObject in its Parent's co-ordinate space
	//
	glm::vec3 positionWorld;		/// Position of this GameObject in world co-ordinate space
	glm::quat orientationWorld;		/// Rotation (a Quaternion) of this GameObject in world co-ordinate space
	glm::vec3 scaleWorld;			/// Scale of this GameObject in world co-ordinate space

	glm::vec3 forward;
	glm::vec3 left;
	glm::vec3 up;

	glm::mat4 modelMatrix;
	glm::mat4 modelMatrixCumulative;

	// Friended to GameObject so as to not have to expose updateModelMatrix()
	friend class GameObject;
};

#endif // TRANSFORM_H
