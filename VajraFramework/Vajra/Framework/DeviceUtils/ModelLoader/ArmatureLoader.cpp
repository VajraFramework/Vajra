#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Armature/Bone.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/DeviceUtils/ModelLoader/ArmatureLoader.h"
#include "Vajra/Framework/DeviceUtils/ModelLoader/Declarations.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/FileUtilities.h"
#include "Vajra/Utilities/Utilities.h"

#include "Libraries/glm/glm.hpp"

#include <map>
#include <vector>

void ArmatureLoader::LoadArmatureFromArmatureFile(const char* armatureFilePath,
												  Armature* out_armature) {

	FRAMEWORK->GetLogger()->dbglog("\nLoading armature from armature file: %s", armatureFilePath);

	std::ifstream armatureFile(armatureFilePath);
	VERIFY(armatureFile.is_open(), "Successfully opened armature file at %s", armatureFilePath);

	{
		int armatureFormatVersionNumber = -1;
		armatureFile >> armatureFormatVersionNumber;
		VERIFY(armatureFormatVersionNumber == ARMATURE_FORMAT_VERSION_NUMBER, "Armature format version number (%d) matches expected (%d)", armatureFormatVersionNumber, ARMATURE_FORMAT_VERSION_NUMBER);
	}


	std::map<std::string /* bone name */, std::string /* parent bone name */> boneParentNames;
	std::map<std::string /* bone name */, std::vector<std::string> /* child bone names */> boneChildNames;

	{

		int numBones;
		armatureFile >> numBones;

		ASSERT(numBones <= MAX_BONES, "Number of bones (%d) is <= max supported (%d)", numBones, MAX_BONES);

		for (int boneIdx = 0; boneIdx < numBones; ++boneIdx) {

			Bone* bone = new Bone();

			std::string boneName;
			armatureFile >> boneName;
			bone->SetName(boneName);

			unsigned int boneId;
			armatureFile >> boneId;
			bone->SetId(boneId);

			// Add bone to armature.
			out_armature->AddBone(bone);

			std::string boneParentName;
			armatureFile >> boneParentName;
			boneParentNames[bone->GetName()] = boneParentName;

			int numChildren;
			armatureFile >> numChildren;
			for (int childIdx = 0; childIdx < numChildren; ++childIdx) {
				std::string childName;
				armatureFile >> childName;
				boneChildNames[bone->GetName()].push_back(childName);
			}

			glm::mat4x4 bindPoseMatrixGlobal = ReadGlmMat4x4FromFile(armatureFile);
			bone->SetBindPoseMatrixGlobal(bindPoseMatrixGlobal);
		}
	}

	// Done adding all bones. Manage the heirarchy.
	{
		std::string rootBoneName;
		armatureFile >> rootBoneName;
		//
		out_armature->SetRootBoneByName(rootBoneName);

		// Manage bone parents:
		for (auto it = boneParentNames.begin(); it != boneParentNames.end(); ++it) {
			std::string boneName = it->first;
			std::string boneParentName = it->second;

			Bone* bone = out_armature->GetBoneByName(boneName);
			Bone* boneParent = out_armature->GetBoneByName(boneParentName);

			if (bone != nullptr && boneParent != nullptr) {
				bone->SetParent(boneParent);
			}
		}

		// Manage bone children:
		for (auto it = boneChildNames.begin(); it != boneChildNames.end(); ++it) {
			std::string boneName = it->first;
			std::vector<std::string> childNames = it->second;

			Bone* bone = out_armature->GetBoneByName(boneName);

			if (bone != nullptr) {
				for (std::string childName : childNames) {
					Bone* boneChild = out_armature->GetBoneByName(childName);
					if (boneChild != nullptr) {
						bone->AddChild(boneChild);
					}
				}
			}
		}
	}

	FRAMEWORK->GetLogger()->dbglog("\nDone processing armature file %s", armatureFilePath);
}
