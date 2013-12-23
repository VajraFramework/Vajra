#include "ExampleGame/Test/TestFile.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

int TestFuntion() {
	FRAMEWORK->GetLogger()->dbglog("\nIn TestFunction()");
#if 1
	{
		GameObject* camera = new GameObject();
		camera->AddComponent<ShadyCamera>();
		camera->GetTransform()->SetPosition(4.0f, 4.0f, 4.0f);
		camera->GetTransform()->LookAt(0.0f, 0.0f, 0.0f);
		ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(camera->GetId());
		ENGINE->GetSceneGraph()->SetMainCameraId(camera->GetId());
	}
#endif
#if 0
	GameObject* wavybox = ENGINE->GetSceneGraph()->GetGameObjectById(109);
	if (wavybox != nullptr) {
		Transform* transform = wavybox->GetTransform();
		transform->Scale(4.0f);
	}
#endif
	{
#if 1
		Object* gridManager = new Object();
		GridManager* gridMgrComp = gridManager->AddComponent<GridManager>();
		gridMgrComp->GenerateTerrainFromFile("noninjas.txt");
#endif
	}
 
	return 4;
}
