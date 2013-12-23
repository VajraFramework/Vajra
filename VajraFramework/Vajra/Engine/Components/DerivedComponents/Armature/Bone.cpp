#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Bone.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Utilities/MathUtilities.h"
#include "Vajra/Utilities/Utilities.h"

#include "Libraries/glm/gtc/matrix_inverse.hpp"


Bone::Bone() {
	this->init();
}

Bone::~Bone() {
	this->destroy();
}

std::string Bone::GetName() {
	return this->name;
}

void Bone::SetName(std::string name) {
	this->name = name;
}

void Bone::Rotate(float angleInDegrees, glm::vec3 axis, bool boneSpace /* = false */) {
	if (!boneSpace) {
		// Axis specified in object space coordinates
		// Convert it to boneSPace
		glm::vec4 newAxis = this->toBoneMatrix * glm::vec4(axis.x, axis.y, axis.z, 0.0f);
		axis = glm::vec3(newAxis.x, newAxis.y, newAxis.z);
	}

	if (axis != ZERO_VEC3) {
		this->localRotationMatrix = this->localRotationMatrix * glm::rotate(angleInDegrees, axis);
	}
}

void Bone::Translate(float distance, glm::vec3 along, bool boneSpace /* = false */) {
	if (!boneSpace) {
		// Axis specified in object space coordinates
		// Convert it to boneSPace
		glm::vec4 newAxis = this->toBoneMatrix * glm::vec4(along.x, along.y, along.z, 0.0f);
		along = glm::vec3(newAxis.x, newAxis.y, newAxis.z);
	}

	along = glm::normalize(along);
	this->localTranslationMatrix = this->localTranslationMatrix * glm::translate(distance * along);
}

void Bone::updateBoneMatrices_recursive() {

	glm::mat4 matrixToPropogate = this->toWorldMatrix *
								  this->localRotationMatrix * this->localTranslationMatrix *
								  this->toBoneMatrix;
	//
	for (Bone* childBone : this->children) {
		childBone->propogateRawMatrixToChildren(matrixToPropogate);
	}

	glm::mat4 finalBoneTransformMatrix = this->localMatrixCumulative *
										 this->toWorldMatrix *
										 this->localRotationMatrix * this->localTranslationMatrix;

	this->armature->finalBoneTransforms[this->id] = finalBoneTransformMatrix *
													this->toBoneMatrix;

#if DRAW_BONES
	this->visualizer->GetTransform()->SetModelMatrixCumulative(((GameObject*)this->armature->GetObject())->GetTransform()->GetModelMatrixCumulative() *
																finalBoneTransformMatrix);
#endif


	// Propogate bone matrix updates to children:
	for (Bone* childBone : this->children) {
		childBone->updateBoneMatrices_recursive();
	}
}

void Bone::propogateRawMatrixToChildren(glm::mat4 rawMatrix) {
	this->localMatrixCumulative = this->localMatrixCumulative * rawMatrix;

	for (Bone* childBone : this->children) {
		childBone->propogateRawMatrixToChildren(rawMatrix);
	}
}

void Bone::SetBindPoseMatrixGlobal(glm::mat4x4 m) {
	this->bindPoseMatrixGlobal = m;

	/*
	 * Note: the bindPoseMatrixGlobal here is the matrix with which if we draw any "bone model",
	 * it would end up in the correct place visually
	 */

	this->toWorldMatrix = this->bindPoseMatrixGlobal;
	this->toBoneMatrix  = glm::inverse(this->toWorldMatrix);

	this->updateBoneMatrices_recursive();
}

void Bone::SetParent(Bone* parentBone) {
	// TODO [Cleanup] Add asserts here
	this->parent = parentBone;
}

void Bone::AddChild(Bone* childBone) {
	// TODO [Cleanup] Add asserts here
	this->children.push_back(childBone);
}

void Bone::init() {
	this->parent = nullptr;

	this->bindPoseMatrixGlobal   = IDENTITY_MATRIX;
	//
	this->toBoneMatrix    = IDENTITY_MATRIX;
	this->toWorldMatrix   = IDENTITY_MATRIX;
	//
	this->localRotationMatrix    = IDENTITY_MATRIX;
	this->localTranslationMatrix = IDENTITY_MATRIX;
	this->localMatrixCumulative    = IDENTITY_MATRIX;

#if DRAW_BONES
	this->visualizer = new GameObject();
	MeshRenderer* meshRenderer = this->visualizer->AddComponent<MeshRenderer>();
	meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "bone.model");
	ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(this->visualizer->GetId());
#endif
}

void Bone::destroy() {
}

