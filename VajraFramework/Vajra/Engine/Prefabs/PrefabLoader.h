#ifndef PREFAB_LOADER_H
#define PREFAB_LOADER_H

#include <string>

// Forward Declarations:
class GameObject;
class SceneGraph;

namespace PrefabLoader {

	GameObject* InstantiateGameObjectFromPrefab(std::string pathToPrefabFile, SceneGraph* sceneGraph);

}

#endif // PREFAB_LOADER_H
