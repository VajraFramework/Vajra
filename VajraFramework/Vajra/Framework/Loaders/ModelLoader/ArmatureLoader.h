#ifndef ARMATURE_LOADER_H
#define ARMATURE_LOADER_H

#include <string>

// Forward Declarations:
class Armature;

namespace ArmatureLoader {

void LoadArmatureFromArmatureFile(const char* armatureFilePath,
								  Armature* out_armature);
}

#endif // ARMATURE_LOADER_H
