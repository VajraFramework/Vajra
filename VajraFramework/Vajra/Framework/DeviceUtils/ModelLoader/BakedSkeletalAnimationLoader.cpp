#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/BakedSkeletalAnimationKeyframe/BakedSkeletalAnimationKeyframe.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/DeviceUtils/ModelLoader/Declarations.h"
#include "Vajra/Framework/DeviceUtils/ModelLoader/BakedSkeletalAnimationLoader.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/FileUtilities.h"
#include "Vajra/Utilities/Utilities.h"

static std::string kClipNameString = "CLIPNAME";

namespace AnimationLoader {

void LoadBakedSkeletalAnimationFromAnimclipsFile(const char* clipsFilePath,
										 std::string clipName,
										 std::vector<BakedSkeletalAnimationKeyFrame*>* out_bakedSkeletalAnimationKeyFrames) {

	FRAMEWORK->GetLogger()->dbglog("\nLoading anim clip \"%s\" from animclips file: %s", clipName.c_str(), clipsFilePath);

	std::ifstream clipsFile(clipsFilePath);
	VERIFY(clipsFile.is_open(), "Successfully opened animclips file at %s", clipsFilePath);

	{
		int animationsFormatVersionNumber = -1;
		clipsFile >> animationsFormatVersionNumber;
		VERIFY(animationsFormatVersionNumber == SKELETAL_ANIMATION_FORMAT_VERSION_NUMBER, "Animations format version number (%d) matches expected (%d)", animationsFormatVersionNumber, SKELETAL_ANIMATION_FORMAT_VERSION_NUMBER);
	}

	{
		std::string animationTypeString;
		clipsFile >> animationTypeString;
		VERIFY(animationTypeString == ANIMATION_TYPE_STRING_BAKED_SKELETAL, "Animation type matches (%s)", animationTypeString.c_str());
	}

	int numBones;
	{
		clipsFile >> numBones;
		VERIFY(numBones < MAX_BONES, "Number of bones, %d, is less than MAX_BONES, %d", numBones, MAX_BONES);
	}

	{
		ReadTextFileTillStringToken(clipsFile, kClipNameString + "#" + clipName);
		VERIFY_LOG(clipsFile.good(), "Clip name \"%s\" found in clips file \"%s\"", clipName.c_str(), clipsFilePath);
	}

	int numKeyFrames;
	{
		clipsFile >> numKeyFrames;
		FRAMEWORK->GetLogger()->dbglog("\nNumber of Key frames found: %d", numKeyFrames);
	}

	ASSERT(out_bakedSkeletalAnimationKeyFrames->size() == 0, "out parameter passed in doesn't contain any data");
	out_bakedSkeletalAnimationKeyFrames->clear();

	while ((numKeyFrames--) > 0) {
		BakedSkeletalAnimationKeyFrame* newKeyFrame = new BakedSkeletalAnimationKeyFrame();

		float time;
		glm::mat4 bakedBoneMatrix_temp;

		clipsFile >> time;
		newKeyFrame->SetTime(time);

		for (int boneIdx = 0; boneIdx < numBones; ++boneIdx) {
			bakedBoneMatrix_temp = ReadGlmMat4x4FromFile(clipsFile);
			newKeyFrame->SetBakedBoneMatrixForBone(bakedBoneMatrix_temp, boneIdx);
		}

		out_bakedSkeletalAnimationKeyFrames->push_back(newKeyFrame);
	}

	FRAMEWORK->GetLogger()->dbglog("\nDone processing animclips file %s", clipsFilePath);
}

};
