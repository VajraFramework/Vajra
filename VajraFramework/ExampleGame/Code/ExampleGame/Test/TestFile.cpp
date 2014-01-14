#include "ExampleGame/Test/TestFile.h"
#include "ExampleGame/Components/GameScripts/SampleGameScript.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/Grid/GridZone.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
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
		SINGLETONS->GetLevelManager()->LoadLevelFromFile(FRAMEWORK->GetFileSystemUtils()->GetDeviceBaseResourcesPath() + "levels/ExampleLevel.lvl");

		GameObject* testZone = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(testZone->GetId());
		GridZone* zone = testZone->AddComponent<GridZone>();
		zone->SetZoneBounds(3, 0, 5, 5);
#endif
	}
	{
#if 0
		GameObject* camera = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(camera->GetId());
		ShadyCamera* cameraComponent = camera->AddComponent<ShadyCamera>();
		cameraComponent->SetCameraType(CAMERA_TYPE_PERSPECTIVE);
		ENGINE->GetSceneGraph3D()->SetMainCameraId(camera->GetId());
		cameraComponent->SetGridManager(SINGLETONS->GetGridManager());
		//cameraComponent->PanTo(0.0f, 0.0f);
		cameraComponent->MoveToRoom(0.0f, 0.0f);
		//cameraComponent->ZoomToOverview();

		GameObject* walker = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(walker->GetId());
#endif
	}
	{
#if 0
		GameObject* testGameScript = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(testGameScript->GetId());
		MeshRenderer* meshRenderer = testGameScript->AddComponent<MeshRenderer>();
		meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "Suzanne.model");
		testGameScript->AddComponent<SampleGameScript>();
		GridNavigator* gNav = testGameScript->AddComponent<GridNavigator>();
		gNav->SetGridPosition(0, 2);
#endif
	}
	{
#if 0
		GameObject* testGameScript = new GameObject(ENGINE->GetSceneGraph3D());
		ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(testGameScript->GetId());
		MeshRenderer* meshRenderer = testGameScript->AddComponent<MeshRenderer>();
		meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "Suzanne.model");
		testGameScript->AddComponent<SampleGameScript>();
		GridNavigator* gNav = testGameScript->AddComponent<GridNavigator>();
		gNav->SetGridPosition(8, 2);
#endif
	}
 
	return 4;
}
