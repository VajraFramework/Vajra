#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/RigidAnimation/RigidAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Placeholder/Tesserakonteres.h"

#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtx/transform.hpp"
#include "Libraries/glm/gtc/type_ptr.hpp"

#include <vector>

namespace Tesserakonteres {

	void initGameObjectsForScene() {
		GameObject* parent = nullptr;
		GameObject* child = nullptr;
		{
			GameObject* gameObject = new GameObject();
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "Suzanne.model");
			ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(gameObject->GetId());
			parent = gameObject;

			Transform* transform = parent->GetTransform();
			transform->Scale(0.2f, 0.2f, 0.2f);
		}
		{
			GameObject* gameObject = new GameObject();
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "TexturedCube.model");
			ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(gameObject->GetId());
			child = gameObject;

			parent->AddChild(child->GetId());
			Transform* transform = child->GetTransform();
			transform->Translate(1.5f, transform->GetForward());
			transform->Rotate(-90.0f, transform->GetLeft());
			transform->Scale(0.4f, 0.4f, 0.4f);
		}
		{
			GameObject* gameObject = new GameObject();
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "pCube1.model");
			//
			RigidAnimation* rigidAnimation = gameObject->AddComponent<RigidAnimation>();
			rigidAnimation->AddAnimationClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAnimationResourcesPath() + "simpleanimation.animclips#clip0");
			rigidAnimation->AddAnimationClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAnimationResourcesPath() + "simpleanimation.animclips#clip1");
			AnimationClip* animationClip = rigidAnimation->GetAnimationClip("clip1");
			animationClip->SetPlaybackSpeed(2.0f);
			animationClip->SetLooping(true);
			rigidAnimation->PlayAnimationClip("clip1");

			ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(gameObject->GetId());
			Transform* transform = gameObject->GetTransform();
			transform->SetPosition(1.0f, 0.0f, 0.0f);
		}
		{
			GameObject* camera = new GameObject();
			/* Camera* cameraComponent = */ camera->AddComponent<Camera>();
			camera->GetTransform()->SetPosition(2.0f, 2.0f, 2.0f);
			// camera->GetTransform()->SetOrientation(-45.0f, camera->GetTransform()->GetUp());
			// camera->GetTransform()->Rotate(45.0f, camera->GetTransform()->GetLeft());
			camera->GetTransform()->LookAt(0.0f, 0.0f, 0.0f);
			ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(camera->GetId());
			ENGINE->GetSceneGraph()->SetMainCameraId(camera->GetId());
		}
		{
			GameObject* dlight = new GameObject();
			DirectionalLight* dlightComponent = dlight->AddComponent<DirectionalLight>();
			dlight->GetTransform()->SetPosition(1.0f, 0.0f, 0.0f);
			ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(dlight->GetId());
			ENGINE->GetSceneGraph()->SetMainDirectionalLightId(dlight->GetId());
			//
			dlightComponent->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
			dlightComponent->SetDiffuseColor(0.7f, 0.7f, 0.7f, 1.0f);
		}
	}
}
