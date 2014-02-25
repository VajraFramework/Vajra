#ifndef DECAL_GENERATOR_H
#define DECAL_GENERATOR_H

#include <string>

// Forward Declarations:
class GameObject;
class SceneGraph;

namespace DecalGenerator {
	GameObject* GetDecalFromDecalType(std::string decalType, SceneGraph* scenegraph);
};

#endif // DECAL_GENERATOR_H
