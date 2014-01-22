#include "Vajra/Engine/Components/DerivedComponents/Renderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Prefabs/ComponentMapperInterface/ComponentMapperInterface.h"
#include "Vajra/Engine/Prefabs/PrefabLoader.h"
#include "Vajra/Engine/Prefabs/ParsingTags.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/XmlParser/XmlParser.h"
#include "Vajra/Utilities/Utilities.h"

void LoadComponentFromComponentNodeInPrefab(GameObject* gameObject, XmlNode* componentNode) {
	std::string componentName = componentNode->GetAttributeValueS(NAME_PROPERTY);
	Component* component = ComponentMapperInterface::GetInstance()->AddNewComponentToGameObjectByComponentName(gameObject, componentName);
	ASSERT(component != nullptr, "Added valid component (%s) to game object", componentName.c_str());

	// Find and add any properties:
	XmlNode* propertyNode = componentNode->GetFirstChildByNodeName(PROPERTY_TAG);
	while (propertyNode != nullptr) {
		std::string propertyName = propertyNode->GetAttributeValueS(NAME_PROPERTY);

		// Find and pass any fields to the property:
		std::vector<std::string> argv;
		XmlNode* fieldNode = propertyNode->GetFirstChildByNodeName(FIELD_TAG);
		while (fieldNode) {
			// TODO [Implement] Support field names
			std::string value = fieldNode->GetValue();
			argv.push_back(value);
			fieldNode = fieldNode->GetNextSiblingByNodeName(FIELD_TAG);
		}
		ComponentMapperInterface::GetInstance()->InitializePropertyByComponentAndPropertyNames(gameObject, componentName, propertyName, argv);
		propertyNode = propertyNode->GetNextSiblingByNodeName(PROPERTY_TAG);
	}
}

void loadGameObjectFromGameObjectNodeInPrefab(GameObject* gameObject, XmlNode* gameobjectNode) {

	// Find and add any components:
	XmlNode* componentNode = gameobjectNode->GetFirstChildByNodeName(COMPONENT_TAG);
	while (componentNode != nullptr) {
		LoadComponentFromComponentNodeInPrefab(gameObject, componentNode);

		componentNode = componentNode->GetNextSiblingByNodeName(COMPONENT_TAG);
	}

	// Find and add any child game objects:
	XmlNode* childGameobjectNode = gameobjectNode->GetFirstChildByNodeName(GAMEOBJECT_TAG);
	while (childGameobjectNode) {
		GameObject* child = new GameObject(gameObject->GetParentSceneGraph());
		gameObject->AddChild(child->GetId());
		loadGameObjectFromGameObjectNodeInPrefab(child, childGameobjectNode);
		//
		childGameobjectNode = childGameobjectNode->GetNextSiblingByNodeName(GAMEOBJECT_TAG);
	}
}

GameObject* PrefabLoader::InstantiateGameObjectFromPrefab(std::string pathToPrefabFile, SceneGraph* sceneGraph) {

	FRAMEWORK->GetLogger()->dbglog("\nLoading prefab from prefab file: %s", pathToPrefabFile.c_str());

	XmlParser* parser = new XmlParser();
	parser->ParseXmlFile(pathToPrefabFile);

#ifdef DEBUG
	parser->Print();
#endif // DEBUG


	XmlTree* xmlTree = parser->GetXmlTree();
	ASSERT(xmlTree != nullptr, "Got valid xmlTree from parser for prefab file %s", pathToPrefabFile.c_str());
	XmlNode* prefabNode = xmlTree->GetRootNode();
	ASSERT(prefabNode != nullptr && prefabNode->GetName() == PREFAB_TAG, "Got valid prefab node from xml tree for prefab file %s", pathToPrefabFile.c_str());

	// TODO [Implement] Support more than 1 game object in the same prefab, maybe as children of the root game object
	XmlNode* gameobjectNode = prefabNode->GetFirstChildByNodeName(GAMEOBJECT_TAG);
	ASSERT(gameobjectNode != nullptr, "Got valid game object node from xml tree for prefab file %s", pathToPrefabFile.c_str());
	GameObject* gameObject = new GameObject(sceneGraph);
	sceneGraph->GetRootGameObject()->AddChild(gameObject->GetId());
	loadGameObjectFromGameObjectNodeInPrefab(gameObject, gameobjectNode);

	return gameObject;
}
