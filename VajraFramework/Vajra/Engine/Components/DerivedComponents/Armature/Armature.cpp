#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Bone.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/ModelLoader/ArmatureLoader.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Utilities/FileUtilities.h"
#include "Vajra/Utilities/MathUtilities.h"
#include "Vajra/Utilities/Utilities.h"

#include "Libraries/glm/gtx/transform.hpp"

#include <fstream>

unsigned int Armature::componentTypeId = COMPONENT_TYPE_ID_ARMATURE;

Armature::Armature() : Component() {
	this->init();
}

Armature::Armature(Object* object_) : Component(object_) {
	this->init();
}

Armature::~Armature() {
	this->destroy();
}

void Armature::HandleMessage(Message* message) {
	// FRAMEWORK->GetLogger()->dbglog("\nArmature got msg of type %d", message->GetMessageType());

	switch (message->GetMessageType()) {

	case MESSAGE_TYPE_FRAME_EVENT:
		this->updateBoneMatrices();
		break;

	default:
		FRAMEWORK->GetLogger()->dbglog("\nArmature got unnecessary msg of type %d", message->GetMessageType());
	}
}

void Armature::Bind() {
	GLint boneTransformsHandle = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->GetHandle(SHADER_VARIABLE_VARIABLENAME_boneTransforms);
    // glUniformMatrix4fv(boneTransformsHandle, MAX_BONES, GL_FALSE, glm::value_ptr(this->finalBoneTransforms[0]));
    glUniformMatrix4fv(boneTransformsHandle, MAX_BONES, GL_FALSE, glm::value_ptr(this->otherFinalBoneTransformsSet[19]->finalBoneTransforms[0]));

	GLint otherBoneTransformsHandle = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->GetHandle(SHADER_VARIABLE_VARIABLENAME_otherBoneTransforms);
    glUniformMatrix4fv(otherBoneTransformsHandle, MAX_BONES, GL_FALSE, glm::value_ptr(this->otherFinalBoneTransformsSet[20]->finalBoneTransforms[0]));

    static int interp = 0;
    interp = (interp + 1) % 200;
	GLint interpHandle = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->GetHandle(SHADER_VARIABLE_VARIABLENAME_interp);
	glUniform2f(interpHandle, interp / 200.0f, 0.0f);
}

void Armature::resetFinalBoneTransforms() {
	for (int i = 0; i < MAX_BONES; ++i) {
		this->finalBoneTransforms[i] = IDENTITY_MATRIX;
	}

	for (int i = 0; i < 60; ++i) {
		FinalBoneTransformsSet* finalBoneTransformsSet = new FinalBoneTransformsSet();
		for (int i = 0; i < MAX_BONES; ++i) {
			finalBoneTransformsSet->finalBoneTransforms[i] = IDENTITY_MATRIX;
		}
		this->otherFinalBoneTransformsSet.push_back(finalBoneTransformsSet);
	}
}

void Armature::updateBoneMatrices() {

	for (auto bone_it = this->bones.begin(); bone_it != this->bones.end(); ++bone_it) {
		Bone* bone = bone_it->second;
		bone->localMatrixCumulative = IDENTITY_MATRIX;
	}

	// this->rootBone->updateBoneMatrices_recursive();

	// if (ENGINE->GetTimer()->GetFrameNumber()%600 == 1) {
		// static int qwe = 0;
		// FRAMEWORK->GetLogger()->dbglog("\nPRINTING BONE MATRICES FOR FRAME NUMBER: %d", qwe);
		// this->DumpBoneKeyframes();
		// qwe++;
	// }
}

void Armature::AddBone(Bone* newBone) {
	this->bones[newBone->id] = newBone;
	newBone->armature = this;
}

Bone* Armature::GetBoneById(unsigned int boneId) {
	ASSERT(this->bones.find(boneId) != this->bones.end(), "Bone found (id = %u)", boneId);
	return this->bones[boneId];
}

Bone* Armature::GetBoneByName(std::string boneName) {
	for (auto bone_it = this->bones.begin(); bone_it != this->bones.end(); ++bone_it) {
		Bone* bone = bone_it->second;
		if (bone->name == boneName) {
			return bone;
		}
	}
	return nullptr;
}

void Armature::InitArmature(std::string armatureFilePath) {
	ArmatureLoader::LoadArmatureFromArmatureFile(armatureFilePath.c_str(), this);
}

void Armature::SetRootBoneByName(std::string rootBoneName) {
	Bone* rootBone = this->GetBoneByName(rootBoneName);
	ASSERT(rootBone != nullptr, "Got root bone (name = %s)", rootBoneName.c_str());
	this->rootBone = rootBone;
}


void Armature::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->resetFinalBoneTransforms();

	// TODO [Hack] Can this be done better?
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void Armature::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void Armature::DumpBoneKeyframes() {
	for (unsigned int i = 0; i < this->bones.size(); ++i) {
		FRAMEWORK->GetLogger()->dbglog("\nMatrix for bone: %d", i);
		printGlmMat4(this->finalBoneTransforms[i]);
	}
}

void Armature::ReadOtherFinalBoneTransformsFromFile(std::string filePath) {
	std::ifstream file(filePath.c_str());
	ASSERT(file.good(), "\nSuccessfully opened file at path: %s", filePath.c_str());

	int versionNumber;
	file >> versionNumber;

	int numBoneMatrices;
	file >> numBoneMatrices;

	int numMatrixSets;	// numSkeletalKeyframes
	file >> numMatrixSets;

	for (int s = 0; s < numMatrixSets; ++s) {

		float time;
		file >> time;

		for (int i = 0; i < numBoneMatrices; ++i) {
			this->otherFinalBoneTransformsSet[s]->finalBoneTransforms[i] = ReadGlmMat4x4FromFile(file);
		}
	}
}
