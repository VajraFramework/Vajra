#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/MathUtilities.h"

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

void Transform::Update() {
	// TODO [Implement]
}

void Transform::Draw() {
	Camera* camera = ENGINE->GetSceneGraph()->GetMainCamera();
	ASSERT(camera != nullptr, "Could get main camera for the scene");

	glm::mat4 mvpMatrix = camera->GetProjMatrix() * camera->GetViewMatrix() * this->modelMatrixCumulative;
    FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->SetMVPMatrixHandle(mvpMatrix);
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

void Transform::Translate(glm::vec3 diff) {
	this->setPosition(this->position + diff);
}

void Transform::Translate(float distance, glm::vec3 along) {
	this->setPosition(this->position + (distance * glm::normalize(along)));
}

void Transform::Rotate(float angleInDegrees, glm::vec3 axis) {
	this->setOrientation(glm::angleAxis(angleInDegrees, axis) * this->orientation);
}

void Transform::Scale(float x, float y, float z) {
	this->setScale(glm::vec3(this->scale.x * x, this->scale.y * y, this->scale.z * z));
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
	// TODO [Implement] this->modelMatrixCumulative
	this->modelMatrixCumulative = this->modelMatrix;

	this->forward = glm::normalize(this->orientation * ZAXIS);
	this->left    = glm::normalize(this->orientation * XAXIS);
	this->up      = glm::normalize(this->orientation * YAXIS);
}

void Transform::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->position = ZERO_VEC3;
	this->orientation = IDENTITY_QUATERNION;
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);

	this->forward = ZAXIS;
	this->left    = XAXIS;
	this->up      = YAXIS;

	this->modelMatrix = IDENTITY_MATRIX;
	this->modelMatrixCumulative = IDENTITY_MATRIX;
}

void Transform::destroy() {
}
