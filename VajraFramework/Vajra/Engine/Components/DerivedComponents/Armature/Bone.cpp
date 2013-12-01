#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Bone.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Utilities/MathUtilities.h"
#include "Vajra/Utilities/Utilities.h"


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
	glm::vec3 newAxis;
	if (!boneSpace) {
		// Axis specified in object space coordinates
		// Convert it to boneSPace
		ASSERT(0, "Implement");
	} else {
		newAxis = axis;
	}

	if (newAxis != ZERO_VEC3) {
		this->localRotationMatrix = this->localRotationMatrix * glm::rotate(angleInDegrees, newAxis);

		this->updateBoneMatrices();
	}
}

void Bone::Translate(float /* distance */, glm::vec3 /* along */) {
	ASSERT(0, "Implement");
}

void Bone::updateBoneMatrices() {
	if (this->parent != nullptr) {
		this->localRotationMatrixCumulative = this->parent->localRotationMatrixCumulative * this->localRotationMatrix;
		this->localTranslationMatrixCumulative = this->parent->localTranslationMatrixCumulative * this->localTranslationMatrix;
	} else {
		this->localRotationMatrixCumulative = this->localRotationMatrix;
		this->localTranslationMatrixCumulative = this->localTranslationMatrix;
	}

#if DRAW_BONES
	// TODO [Implement] This doesn't really work since there is really no way for the parent's transform changes to ripple to the bones visualizers
	glm::mat4 visualizerMatrix = ((GameObject*)(this->armature->GetObject()))->GetTransform()->GetModelMatrixCumulative() *
								 this->bindPoseMatrixGlobal * this->localRotationMatrixCumulative * this->localTranslationMatrixCumulative;
	this->visualizer->GetTransform()->SetModelMatrixCumulative(visualizerMatrix);
#endif

	this->rippleBoneMatrixUpdates();
}

void Bone::rippleBoneMatrixUpdates() {
	// Propogate bone matrix updates to children:
	for (Bone* childBone : this->children) {
		childBone->updateBoneMatrices();
	}
}

void Bone::SetBindPoseMatrixGlobal(glm::mat4x4 m) {
	this->bindPoseMatrixGlobal = m;
#if DRAW_BONES
	this->updateBoneMatrices();
#endif
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
	this->localRotationMatrixCumulative    = IDENTITY_MATRIX;
	this->localTranslationMatrixCumulative = IDENTITY_MATRIX;

#if DRAW_BONES
	this->visualizer = new GameObject();
	MeshRenderer* meshRenderer = this->visualizer->AddComponent<MeshRenderer>();
	meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "bone.model");
	ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(this->visualizer->GetId());
#endif
}

void Bone::destroy() {
}
