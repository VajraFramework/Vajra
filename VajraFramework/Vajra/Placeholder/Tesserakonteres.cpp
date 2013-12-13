#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioSource.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/RigidAnimation/RigidAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Placeholder/Tesserakonteres.h"

#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtx/transform.hpp"
#include "Libraries/glm/gtc/type_ptr.hpp"

#include <vector>


namespace Tesserakonteres {

	void tweenCallback(ObjectIdType gameObjectId, std::string tweenClipName) {
		FRAMEWORK->GetLogger()->dbglog("\nIn tweenCallback() with %d, %s\n", gameObjectId, tweenClipName.c_str());
	}

	void tweenNumberCallback(float normalizedProgress, std::string tweenName) {
		// FRAMEWORK->GetLogger()->dbglog("\nIn tweenNumberCallback() with normalized progress %f", normalizedProgress);
		if (tweenName == "tween_stubby_arrows") {
			GameObject* gameObject = ENGINE->GetSceneGraph()->GetGameObjectById(112);
			if (gameObject != nullptr) {
				gameObject->GetTransform()->SetPosition(0.0f, 0.0f, normalizedProgress);
			}
		}
	}

	void initGameObjectsForScene() {

		GameObject* parent = nullptr;
		GameObject* child = nullptr;
		GameObject* wavybox = nullptr;
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
#if 1
			// Draw a wavybox
			GameObject* gameObject = new GameObject();
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "polySurface5.model");
			ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(gameObject->GetId());
			Armature* armature = gameObject->GetComponent<Armature>();
			if (armature != nullptr) {
				armature->ReadOtherFinalBoneTransformsFromFile(FRAMEWORK->GetFileSystemUtils()->GetDeviceAnimationResourcesPath() + "bakedbonematrices");
			}
			//
			Transform* transform = gameObject->GetTransform();
			transform->Scale(0.2f);
			transform->Translate(1.0f, transform->GetForward());
			wavybox = gameObject;
#endif
			//
#if 0
			RigidAnimation* rigidAnimation = gameObject->AddComponent<RigidAnimation>();
			rigidAnimation->AddAnimationClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAnimationResourcesPath() + "simpleanimation.animclips#clip0");
			rigidAnimation->AddAnimationClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAnimationResourcesPath() + "simpleanimation.animclips#clip1");
			AnimationClip* animationClip = rigidAnimation->GetAnimationClip("clip1");
			animationClip->SetPlaybackSpeed(1.0f);
			animationClip->SetLooping(true);
			rigidAnimation->PlayAnimationClip("clip1");
#endif

#if 0
			ENGINE->GetTween()->TweenPosition(gameObject->GetId(), glm::vec3(0.0f, 0.0f, 0.0f),
					                                               glm::vec3(0.0f, 1.0f, 0.0f),
					                                               2.0f, tweenCallback);
#endif
#if 1
			ENGINE->GetTween()->TweenTransform(gameObject->GetId(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
																	glm::angleAxis(0.0f, YAXIS), glm::angleAxis(90.0f, YAXIS),
																	gameObject->GetTransform()->GetScale(), gameObject->GetTransform()->GetScale(),
					                                                2.0f, tweenCallback);
			ENGINE->GetTween()->TweenToNumber(-1.0f, 1.0f, 2.0f, true, "tween_stubby_arrows", tweenNumberCallback);
#endif
		}
		{
			GameObject* gameObject = new GameObject();
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "pTorus1.model");
			//
#if 1
			RigidAnimation* rigidAnimation = gameObject->AddComponent<RigidAnimation>();
			rigidAnimation->AddAnimationClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAnimationResourcesPath() + "pTorus1.animclips#Take_0010");
			AnimationClip* animationClip = rigidAnimation->GetAnimationClip("Take_0010");
			animationClip->SetPlaybackSpeed(0.2f);
			animationClip->SetLooping(true);
			rigidAnimation->PlayAnimationClip("Take_0010");

			ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(gameObject->GetId());
			Transform* transform = gameObject->GetTransform();
			transform->SetScale(0.1f, 0.1f, 0.1f);
#endif
		}
		{
			GameObject* gameObject = new GameObject();
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "stubbyarrows.model");
			ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(gameObject->GetId());
			FRAMEWORK->GetLogger()->dbglog("\nStubby Arrows id: %d", gameObject->GetId());

			Transform* transform = gameObject->GetTransform();
			transform->Scale(0.6f, 0.6f, 0.6f);
			transform->SetPosition(wavybox->GetTransform()->GetPositionWorld());
		}
		{
			GameObject* camera = new GameObject();
			/* Camera* cameraComponent = */ camera->AddComponent<Camera>();
			camera->GetTransform()->SetPosition(4.0f, 4.0f, 4.0f);
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
		{
#if 0
			GameObject* gameObject = new GameObject();
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "bone.model");
			ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(gameObject->GetId());
			parent = gameObject;

			Transform* transform = parent->GetTransform();
			transform->Scale(0.2f);
#endif
		}
		{
#if 1
			GameObject* noiseMaker = new GameObject();
			AudioSource* source = noiseMaker->AddComponent<AudioSource>();
			source->SetAudioClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAudioResourcesPath() + "lowAlert(hisfixed).wav");
			source->Play();
#endif
		}

		FRAMEWORK->GetLogger()->dbglog("\nDone loading game objects for the scene");
	}
}
