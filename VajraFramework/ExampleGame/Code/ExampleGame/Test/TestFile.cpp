#include "ExampleGame/Test/TestFile.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/GameScripts/SampleGameScript.h"

#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

int TestFuntion() {
	FRAMEWORK->GetLogger()->dbglog("\nIn TestFunction()");

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
	{
#if 1
		GameObject* testGameScript = new GameObject();
		testGameScript->AddComponent<SampleGameScript>();
#endif
	}
 
	return 4;
}
