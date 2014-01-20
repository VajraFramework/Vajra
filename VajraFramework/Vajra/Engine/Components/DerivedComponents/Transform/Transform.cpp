#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/MathUtilities.h"

#include "Libraries/glm/gtc/matrix_inverse.hpp"
#include "Libraries/glm/gtx/vector_angle.hpp"

unsigned int Transform::componentTypeId = COMPONENT_TYPE_ID_TRANSFORM;

Transform::Transform() : Component() {
	this->init();
}

Transform::Transform(Object* object_) : Component(object_) {
	this->init();
}

Transform::~Transform() {
	this->destroy();
}

void Transform::HandleMessage(MessageChunk messageChunk) {
	// TODO [Implement] Implement Update logic here, but on SET_TRANSFORM and similar messages
	switch (messageChunk->GetMessageType()) {

	default:
		break;
	}
}

void Transform::Draw() {
	Camera* camera = nullptr;
	GameObject* gameObject = (GameObject*)this->GetObject();
	if (gameObject != nullptr) {
		camera = gameObject->GetParentSceneGraph()->GetMainCamera();
	} else {
		// TODO [Hack] This exists only to make debug drawing work
		camera = ENGINE->GetSceneGraph3D()->GetMainCamera();
	}
	ASSERT(camera != nullptr, "Could get main camera for the scene");

	ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_mvpMatrix)) {
		glm::mat4 mvpMatrix = camera->GetProjMatrix() * camera->GetViewMatrix() * this->modelMatrixCumulative;
		GLint mvpMatrixHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_mvpMatrix);
		glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	} 
	else if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_modelMatrix)) {
		glm::mat4 modelMatric = this->modelMatrixCumulative;
		GLint modelMatrixHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_modelMatrix);
		glUniformMatrix4fv(modelMatrixHandle, 1, GL_FALSE, glm::value_ptr(modelMatric));

    	glm::mat4 vpMatrix = camera->GetProjMatrix() * camera->GetViewMatrix();
		GLint vpMatrixHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_vpMatrix);
		glUniformMatrix4fv(vpMatrixHandle, 1, GL_FALSE, glm::value_ptr(vpMatrix));
	}
    //
	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_modelInverseTransposeMatrix)) {
		glm::mat4 modelInverseTransposeMatrix = glm::inverseTranspose(this->modelMatrixCumulative);
		GLint mitMatrixHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_modelInverseTransposeMatrix);
    	glUniformMatrix4fv(mitMatrixHandle, 1, GL_FALSE, glm::value_ptr(modelInverseTransposeMatrix));
	}
}

glm::vec3& Transform::GetPosition() {
	return this->position;
}

glm::quat& Transform::GetOrientation() {
	return this->orientation;
}

glm::vec3& Transform::GetScale() {
	return this->scale;
}

glm::vec3& Transform::GetPositionWorld() {
	return this->positionWorld;
}

glm::quat& Transform::GetOrientationWorld() {
	return this->orientationWorld;
}

glm::vec3& Transform::GetScaleWorld() {
	return this->scaleWorld;
}

void Transform::SetPosition(float x, float y, float z) {
	this->setPosition(glm::vec3(x, y, z));
}

void Transform::SetPosition(glm::vec3 newPosition) {
	this->setPosition(newPosition);
}

void Transform::SetOrientation(float angleInDegrees, float x, float y, float z) {
	this->setOrientation(glm::angleAxis(angleInDegrees, \
										glm::vec3(x, y, z)));
}

void Transform::SetOrientation(float angleInDegrees, glm::vec3 axis) {
	this->setOrientation(glm::angleAxis(angleInDegrees, \
										axis));
}

void Transform::SetOrientation(glm::quat newOrientation) {
	this->setOrientation(newOrientation);
}

void Transform::SetScale(float x, float y, float z) {
	this->setScale(glm::vec3(x, y, z));
}

void Transform::Translate(float x, float y, float z) {
	this->Translate(glm::vec3(x, y, z));
}

void Transform::Translate(glm::vec3 diff) {
	this->setPosition(this->position + diff);
}

void Transform::Translate(float distance, glm::vec3 along) {
	this->setPosition(this->position + (distance * glm::normalize(along)));
}

void Transform::Rotate(float angleInDegrees, float x, float y, float z) {
	this->Rotate(angleInDegrees, glm::vec3(x, y, z));
}

void Transform::Rotate(float angleInDegrees, glm::vec3 axis) {
	this->setOrientation(glm::angleAxis(angleInDegrees, axis) * this->orientation);
}

void Transform::Rotate(glm::quat quaternion) {
	this->setOrientation(quaternion * this->orientation);
}

void Transform::Scale(float x, float y, float z) {
	this->setScale(glm::vec3(this->scale.x * x, this->scale.y * y, this->scale.z * z));
}

void Transform::Scale(glm::vec3 scaleVector) {
	this->setScale(scaleVector);
}

void Transform::Scale(float scaleFactor) {
	this->SetScale(this->scale.x * scaleFactor, this->scale.y * scaleFactor, this->scale.z * scaleFactor);
}

void Transform::LookAt(float point_x, float point_y, float point_z) {
	this->LookAt(glm::vec3(point_x, point_y, point_z));
}

// TODO [Implement] Currently this can only handle points that are in the transform's coordinate space
void Transform::LookAt(glm::vec3 point) {
	glm::vec3 oldUp = this->GetUp();
	{
		// Rotate so that forward faces the point:
		glm::vec3 connectingVector = glm::normalize(point - this->GetPosition());
		glm::vec3 crossProduct = glm::normalize( glm::cross(this->GetForward(), connectingVector) );
		if (crossProduct == ZERO_VEC3) {
			crossProduct = this->up;
		}
		float angleBetweenVectors = glm::orientedAngle(this->GetForward(), connectingVector, crossProduct);
		this->Rotate(angleBetweenVectors, crossProduct);
	}
	{
		// Try to restore up to what it was before the rotation without disturbing forward:
		float angleToUp = glm::orientedAngle(this->GetUp(), oldUp, this->GetForward());
		this->Rotate(angleToUp, this->GetForward());
	}
}

void Transform::setPosition(glm::vec3 newPosition) {
	this->position = newPosition;

	// TODO [Implement] : call this only once at the end of draining all msgs in this frame, maybe
	this->updateModelMatrix();
}

void Transform::setOrientation(glm::quat newOrientation) {
	this->orientation = newOrientation;

	// TODO [Implement] : call this only once at the end of draining all msgs in this frame, maybe
	this->updateModelMatrix();
}

void Transform::setScale(glm::vec3 newScale) {
	this->scale = newScale;

	// TODO [Implement] : call this only once at the end of draining all msgs in this frame, maybe
	this->updateModelMatrix();
}


void Transform::updateModelMatrix() {
	this->modelMatrix = glm::translate(this->position) *
						glm::toMat4(this->orientation) *
						glm::scale(this->scale);

	this->forward = glm::normalize(this->orientation * ZAXIS);
	this->left    = glm::normalize(this->orientation * XAXIS);
	this->up      = glm::normalize(this->orientation * YAXIS);

	// Update modelMatrixCumulative to include parent's transform space:
	this->updateModelMatrixCumulative();
}

void Transform::updateModelMatrixCumulative() {
	GameObject* parent = nullptr;
	if (this->GetObject() != nullptr) {
		parent = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->GetObject()->GetParentId());
	}
	if (parent != nullptr) {
		this->modelMatrixCumulative = parent->GetTransform()->modelMatrixCumulative * this->modelMatrix;
	} else {
		this->modelMatrixCumulative = this->modelMatrix;
	}

	// Update our picture of position, orientation, and scale in world space as well:
	glm::vec4 worldPosition = this->modelMatrixCumulative * ZERO_VEC4_POSITION;
	this->positionWorld    = glm::vec3(worldPosition.x, worldPosition.y, worldPosition.z);
	this->scaleWorld.x     = this->modelMatrixCumulative[0][0];
	this->scaleWorld.y     = this->modelMatrixCumulative[1][1];
	this->scaleWorld.z     = this->modelMatrixCumulative[2][2];
	this->orientationWorld = glm::quat_cast(this->modelMatrixCumulative);

	this->rippleMatrixUpdates();
}

void Transform::rippleMatrixUpdates() {
	if (this->GetObject() != nullptr) {
		// Raise event so that any interested parties are alerted that the transform has changed:
		MessageChunk messageChunk = ENGINE->GetMessageHub()->GetOneFreeMessage();
		messageChunk->SetMessageType(MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT);
		// Send the message to this GameObject
		ENGINE->GetMessageHub()->SendPointcastMessage(messageChunk, this->GetObject()->GetId(), this->GetObject()->GetId());

		// Update this GameObject's children
		std::list<ObjectIdType> children = this->GetObject()->GetChildren();
		for (ObjectIdType& childId : children) {
			GameObject* child = ENGINE->GetSceneGraph3D()->GetGameObjectById(childId);
			if (child != nullptr) {
				child->GetTransform()->updateModelMatrixCumulative();
			}
		}
	}
}

void Transform::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->position = ZERO_VEC3;
	this->orientation = IDENTITY_QUATERNION;
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);

	this->positionWorld = ZERO_VEC3;
	this->orientationWorld = IDENTITY_QUATERNION;
	this->scaleWorld = glm::vec3(1.0f, 1.0f, 1.0f);

	this->forward = ZAXIS;
	this->left    = XAXIS;
	this->up      = YAXIS;

	this->modelMatrix = IDENTITY_MATRIX;
	this->modelMatrixCumulative = IDENTITY_MATRIX;
}

void Transform::destroy() {
}

