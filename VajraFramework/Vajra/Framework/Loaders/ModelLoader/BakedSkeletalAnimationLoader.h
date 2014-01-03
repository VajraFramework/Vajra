#ifndef BAKED_SKELETAL_ANIMATION_LOADER_H
#define BAKED_SKELETAL_ANIMATION_LOADER_H

#include "Libraries/glm/glm.hpp"

#include <fstream>
#include <vector>
#include <string>

namespace AnimationLoader {

void LoadBakedSkeletalAnimationFromAnimclipsFile(const char* clipsFilePath,
										 std::string clipName,
										 std::vector<BakedSkeletalAnimationKeyFrame*>* out_bakedSkeletalAnimationKeyFrames);

}

#endif // BAKED_SKELETAL_ANIMATION_LOADER_H
