#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Bone.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Framework/DeviceUtils/ModelLoader/ArmatureLoader.h"
#include "Vajra/Utilities/MathUtilities.h"
#include "Vajra/Utilities/Utilities.h"

#include "Libraries/glm/gtx/transform.hpp"

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

void Armature::HandleMessage(Message* /* message */) {
}

void Armature::AddBone(Bone* newBone) {
	newBone->id = this->bones.size();
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
}

void Armature::destroy() {
}
