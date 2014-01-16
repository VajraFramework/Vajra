#include "Vajra/Engine/Components/DerivedComponents/Renderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/GameObject/GameObject.h"
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
		// TODO [Hack] Get rid of this once we can add components from xml
		XmlNode* modelNode = prefabNode->GetFirstChildByNodeName(MODEL_TAG);
		ASSERT(modelNode != nullptr, "Got valid model node from xml tree for prefab file %s", pathToPrefabFile.c_str());
		std::string modelName = modelNode->GetAttributeValueS(MODEL_NAME_ATTRIBUTE);
		ASSERT(modelName != "", "Got valid model name from xml tree for prefab file %s", pathToPrefabFile.c_str());
		MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
		meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + modelName);
	}

	{
		// TODO [Hack] Get rid of this once we can add components from xml
		XmlNode* positionNode = prefabNode->GetFirstChildByNodeName(POSITION_TAG);
		ASSERT(positionNode != nullptr, "Got valid position node from xml tree for prefab file %s", pathToPrefabFile.c_str());
		float x = positionNode->GetAttributeValueF(X_ATTRIBUTE);
		float y = positionNode->GetAttributeValueF(Y_ATTRIBUTE);
		float z = positionNode->GetAttributeValueF(Z_ATTRIBUTE);
		Transform* transform = gameObject->GetTransform();
		transform->SetPosition(x, y, z);
	}

	return gameObject;
}
