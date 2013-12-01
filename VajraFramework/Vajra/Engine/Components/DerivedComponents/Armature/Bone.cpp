#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Bone.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
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

void Bone::Rotate(float /* angleInDegrees */, glm::vec3 /* axis */) {
	ASSERT(0, "Implement");
}

void Bone::Translate(float /* distance */, glm::vec3 /* along */) {
	ASSERT(0, "Implement");
}

#if DRAW_BONES
void Bone::Draw() {
	ASSERT(0, "Implement");
}
#endif

void Bone::SetBindPoseMatrixGlobal(glm::mat4x4 m) {
	this->bindPoseMatrixGlobal = m;
#if DRAW_BONES
	// TODO [Implement] This doesn't really work since there is really no way for the parent's transform changes to ripple to the bones visualizers
	glm::mat4 visualizerMatrix = ((GameObject*)(this->armature->GetObject()))->GetTransform()->GetModelMatrixCumulative() *
								 this->bindPoseMatrixGlobal;
	this->visualizer->GetTransform()->SetModelMatrixCumulative(visualizerMatrix);
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

#if DRAW_BONES
	this->visualizer = new GameObject();
	MeshRenderer* meshRenderer = this->visualizer->AddComponent<MeshRenderer>();
	meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "bone.model");
	ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(this->visualizer->GetId());
#endif
}

void Bone::destroy() {
}
