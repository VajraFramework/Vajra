#ifndef RIGID_ANIMATION_LOADER_H
#define RIGID_ANIMATION_LOADER_H

#include "Libraries/glm/glm.hpp"

#include <fstream>
#include <vector>
#include <string>

namespace AnimationLoader {

void LoadRigidAnimationFromAnimclipsFile(const char* clipsFilePath,
										 std::string clipName,
										 std::vector<RigidAnimationKeyFrame*>* out_rigidAnimationKeyFrames);

}

#endif // RIGID_ANIMATION_LOADER_H
