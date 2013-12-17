#include "Vajra/Engine/AssetLibrary/Assets/AnimationAssets/AnimationKeyFrames/RigidAnimationKeyFrame/RigidAnimationKeyFrame.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/DeviceUtils/ModelLoader/Declarations.h"
#include "Vajra/Framework/DeviceUtils/ModelLoader/RigidAnimationLoader.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/FileUtilities.h"
#include "Vajra/Utilities/Utilities.h"

static std::string kClipNameString = "CLIPNAME";

namespace AnimationLoader {

void LoadRigidAnimationFromAnimclipsFile(const char* clipsFilePath,
										 std::string clipName,
										 std::vector<RigidAnimationKeyFrame*>* out_rigidAnimationKeyFrames) {

	FRAMEWORK->GetLogger()->dbglog("\nLoading anim clip \"%s\" from animclips file: %s", clipName.c_str(), clipsFilePath);

	std::ifstream clipsFile(clipsFilePath);
	VERIFY(clipsFile.is_open(), "Successfully opened animclips file at %s", clipsFilePath);

	{
		int animationsFormatVersionNumber = -1;
		clipsFile >> animationsFormatVersionNumber;
		VERIFY(animationsFormatVersionNumber == RIGID_ANIMATION_FORMAT_VERSION_NUMBER, "Animations format version number (%d) matches expected (%d)", animationsFormatVersionNumber, RIGID_ANIMATION_FORMAT_VERSION_NUMBER);
	}

	{
		std::string animationTypeString;
		clipsFile >> animationTypeString;
		VERIFY(animationTypeString == ANIMATION_TYPE_STRING_RIGID, "Animation type matches (%s)", animationTypeString.c_str());
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

	ASSERT(out_rigidAnimationKeyFrames->size() == 0, "out parameter passed in doesn't contain any data");
	out_rigidAnimationKeyFrames->clear();

	while ((numKeyFrames--) > 0) {
		float time;
		glm::vec3 translation;
		glm::vec3 rotation;
		glm::vec3 scaling;

		clipsFile >> time;
		clipsFile >> translation.x >> translation.y >> translation.z;
		clipsFile >> rotation.x >> rotation.y >> rotation.z;
		clipsFile >> scaling.x >> scaling.y >> scaling.z;
		//
		rotation.x *= PI / 180.0f;
		rotation.y *= PI / 180.0f;
		rotation.z *= PI / 180.0f;
		glm::quat rotationQuat(rotation);

		RigidAnimationKeyFrame* newKeyFrame = new RigidAnimationKeyFrame();
		newKeyFrame->SetTime(time);
		newKeyFrame->SetTranslation(translation);
		newKeyFrame->SetRotation(rotationQuat);
		newKeyFrame->SetScaling(scaling);

		out_rigidAnimationKeyFrames->push_back(newKeyFrame);
	}

	FRAMEWORK->GetLogger()->dbglog("\nDone processing animclips file %s", clipsFilePath);
}

};
