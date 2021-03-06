#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Bone.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Loaders/ModelLoader/ArmatureLoader.h"
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

void Armature::HandleMessage(MessageChunk messageChunk) {
	// FRAMEWORK->GetLogger()->dbglog("\nArmature got msg of type %d", messageChunk->GetMessageType());

	switch (messageChunk->GetMessageType()) {

	case MESSAGE_TYPE_FRAME_EVENT:
		this->updateBoneMatrices();
		break;

	default:
		FRAMEWORK->GetLogger()->dbglog("\nArmature got unnecessary msg of type %d", messageChunk->GetMessageType());
	}
}

void Armature::Bind() {
#if USING_RUNTIME_COMPUTED_BONE_MATRICES
	GLint boneTransformsHandle = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet()->GetHandle(SHADER_VARIABLE_VARIABLENAME_boneTransforms);
    GLCALL(glUniformMatrix4fv, boneTransformsHandle, MAX_BONES, GL_FALSE, glm::value_ptr(this->finalBoneTransforms[0]));
#endif // USING_RUNTIME_COMPUTED_BONE_MATRICES
}

void Armature::resetFinalBoneTransforms() {
	for (int i = 0; i < MAX_BONES; ++i) {
		this->finalBoneTransforms[i] = IDENTITY_MATRIX;
	}
}

void Armature::updateBoneMatrices() {
#if USING_RUNTIME_COMPUTED_BONE_MATRICES
	for (auto bone_it = this->bones.begin(); bone_it != this->bones.end(); ++bone_it) {
		Bone* bone = bone_it->second;
		bone->localMatrixCumulative = IDENTITY_MATRIX;
	}

	this->rootBone->updateBoneMatrices_recursive();
#endif // USING_RUNTIME_COMPUTED_BONE_MATRICES

	/*
	if (ENGINE->GetTimer()->GetFrameNumber()%600 == 1) {
		static int keyframeNumberBeingDumped = 0;
		FRAMEWORK->GetLogger()->dbglog("\nPRINTING BONE MATRICES FOR KEY FRAME NUMBER: %d", keyframeNumberBeingDumped);
		this->DumpBoneKeyframes();
		keyframeNumberBeingDumped++;
	}
	*/
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

/*
void Armature::DumpBoneKeyframes() {
	for (unsigned int i = 0; i < this->bones.size(); ++i) {
		FRAMEWORK->GetLogger()->dbglog("\nMatrix for bone: %d", i);
		printGlmMat4(this->finalBoneTransforms[i]);
	}
}
*/
