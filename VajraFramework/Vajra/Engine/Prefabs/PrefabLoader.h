#ifndef PREFAB_LOADER_H
#define PREFAB_LOADER_H

#include <string>

// Forward Declarations:
class GameObject;
class SceneGraph;
class XmlNode;

namespace PrefabLoader {

	GameObject* InstantiateGameObjectFromPrefab       (std::string pathToPrefabFile, SceneGraph* sceneGraph);
	void        LoadComponentFromComponentNodeInPrefab(GameObject* gameObject, XmlNode* componentNode);
	void        LoadTagFromTagsNodeInPrefab(GameObject* gameObject, XmlNode* tagsNode);

}

#endif // PREFAB_LOADER_H
