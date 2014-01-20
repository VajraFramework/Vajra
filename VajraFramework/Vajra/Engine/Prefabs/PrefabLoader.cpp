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

GameObject* PrefabLoader::InstantiateGameObjectFromPrefab(std::string pathToPrefabFile, SceneGraph* sceneGraph) {

	FRAMEWORK->GetLogger()->dbglog("\nLoading prefab from prefab file: %s", pathToPrefabFile.c_str());

	XmlParser* parser = new XmlParser();
	parser->ParseXmlFile(pathToPrefabFile);

#ifdef DEBUG
	parser->Print();
#endif // DEBUG

	GameObject* gameObject = new GameObject(sceneGraph);
	sceneGraph->GetRootGameObject()->AddChild(gameObject->GetId());

	XmlTree* xmlTree = parser->GetXmlTree();
	ASSERT(xmlTree != nullptr, "Got valid xmlTree from parser for prefab file %s", pathToPrefabFile.c_str());
	XmlNode* prefabNode = xmlTree->GetRootNode();
	ASSERT(prefabNode != nullptr && prefabNode->GetName() == PREFAB_TAG, "Got valid prefab node from xml tree for prefab file %s", pathToPrefabFile.c_str());

	{
		// TODO [Implement] Support more than 1 game object in the same prefab, maybe as children of the root game object
		XmlNode* gameobjectNode = prefabNode->GetFirstChildByNodeName(GAMEOBJECT_TAG);
		ASSERT(gameobjectNode != nullptr, "Got valid game object node from xml tree for prefab file %s", pathToPrefabFile.c_str());
		XmlNode* componentNode = gameobjectNode->GetFirstChildByNodeName(COMPONENT_TAG);
		while (componentNode != nullptr) {
			std::string componentName = componentNode->GetAttributeValueS(NAME_PROPERTY);
			Component* component = ComponentMapperInterface::GetInstance()->AddNewComponentToGameObjectByComponentName(gameObject, componentName);
			ASSERT(component != nullptr, "Added valid component (%s) to game object", componentName.c_str());
			XmlNode* propertyNode = componentNode->GetFirstChildByNodeName(PROPERTY_TAG);
			while (propertyNode != nullptr) {
				std::string propertyName = propertyNode->GetAttributeValueS(NAME_PROPERTY);
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
			componentNode = componentNode->GetNextSiblingByNodeName(COMPONENT_TAG);
		}
	}

	return gameObject;
}
