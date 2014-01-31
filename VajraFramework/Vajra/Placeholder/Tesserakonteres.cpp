#include "Vajra/Engine/Components/DerivedComponents/Armature/Armature.h"
#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioSource.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/AnimationClip/AnimationClip.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/BakedSkeletalAnimation/BakedSkeletalAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Animation/RigidAnimation/RigidAnimation.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/UiFontRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/Prefabs/PrefabLoader.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Placeholder/Tesserakonteres.h"
#include "Vajra/Utilities/XmlParser/XmlParser.h"

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
			GameObject* gameObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(115);
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
			GameObject* gameObject = new GameObject(ENGINE->GetSceneGraph3D());
			ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(gameObject->GetId());
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesFolderName() + "Suzanne.model");
			parent = gameObject;

			Transform* transform = parent->GetTransform();
			transform->Scale(0.2f, 0.2f, 0.2f);
		}
		{
			GameObject* gameObject = new GameObject(ENGINE->GetSceneGraph3D());
			ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(gameObject->GetId());
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesFolderName() + "TexturedCube.model");
			child = gameObject;

			parent->AddChild(child->GetId());
			Transform* transform = child->GetTransform();
			transform->Translate(1.5f, transform->GetForward());
			transform->Rotate(-90.0f inRadians, transform->GetLeft());
			transform->Scale(0.4f, 0.4f, 0.4f);
		}
		{
#if 1
			// Draw a wavybox
			GameObject* gameObject = new GameObject(ENGINE->GetSceneGraph3D());
			ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(gameObject->GetId());
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesFolderName() + "polySurface5.model");
			BakedSkeletalAnimation* bakedSkeletalAnimation = gameObject->AddComponent<BakedSkeletalAnimation>();
			bakedSkeletalAnimation->AddAnimationClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAnimationResourcesFolderName() + "SD_Imposter_mesh.skeletalanimation#idle");
			bakedSkeletalAnimation->AddAnimationClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAnimationResourcesFolderName() + "SD_Imposter_mesh.skeletalanimation#twitching");
			bakedSkeletalAnimation->AddAnimationClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAnimationResourcesFolderName() + "SD_Imposter_mesh.skeletalanimation#turning");
			bakedSkeletalAnimation->AddAnimationClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAnimationResourcesFolderName() + "SD_Imposter_mesh.skeletalanimation#nodding");
			std::string animclipToPlay = "twitching";
			AnimationClip* animationClip = bakedSkeletalAnimation->GetAnimationClip(animclipToPlay.c_str());
			animationClip->SetLooping(true);
			animationClip->SetPlaybackSpeed(0.1f);
			bakedSkeletalAnimation->PlayAnimationClip(animclipToPlay.c_str());
			//
			Transform* transform = gameObject->GetTransform();
			transform->Scale(0.08f);
			transform->Translate(1.0f, transform->GetForward());
			// transform->Rotate(180.0f, YAXIS);
			transform->SetPosition(10.0f, 0.0f, -10.0f);
			wavybox = gameObject;
#endif
			//
#if 0
			RigidAnimation* rigidAnimation = gameObject->AddComponent<RigidAnimation>();
			rigidAnimation->AddAnimationClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAnimationResourcesFolderName() + "simpleanimation.animclips#clip0");
			rigidAnimation->AddAnimationClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAnimationResourcesFolderName() + "simpleanimation.animclips#clip1");
			AnimationClip* animationClip = rigidAnimation->GetAnimationClip("clip1");
			animationClip->SetPlaybackSpeed(1.0f);
			animationClip->SetLooping(true);
			rigidAnimation->PlayAnimationClip("clip1");
#endif

#if 0
			ENGINE->GetTween()->TweenPosition(gameObject->GetId(), glm::vec3(-1.0f, 0.0f, 0.0f),
					                                               glm::vec3(-1.0f, 1.0f, 0.0f),
					                                               2.0f, tweenCallback);
#endif
#if 0
			ENGINE->GetTween()->TweenTransform(gameObject->GetId(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
																	glm::angleAxis(0.0f, YAXIS), glm::angleAxis(90.0f, YAXIS),
																	gameObject->GetTransform()->GetScale(), gameObject->GetTransform()->GetScale(),
					                                                2.0f, tweenCallback);
			ENGINE->GetTween()->TweenToNumber(-1.0f, 1.0f, 2.0f, true, "tween_stubby_arrows", tweenNumberCallback);
#endif
		}
		{
			GameObject* gameObject = new GameObject(ENGINE->GetSceneGraph3D());
			ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(gameObject->GetId());
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesFolderName() + "pTorus1.model");

			Transform* transform = gameObject->GetTransform();
			transform->SetScale(0.1f, 0.1f, 0.1f);
			//
#if 1
			RigidAnimation* rigidAnimation = gameObject->AddComponent<RigidAnimation>();
			rigidAnimation->AddAnimationClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAnimationResourcesFolderName() + "pTorus1.animclips#Take_0010");
			AnimationClip* animationClip = rigidAnimation->GetAnimationClip("Take_0010");
			animationClip->SetPlaybackSpeed(0.2f);
			animationClip->SetLooping(true);
			rigidAnimation->PlayAnimationClip("Take_0010");
#endif
		}
		{
			GameObject* gameObject = new GameObject(ENGINE->GetSceneGraph3D());
			ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(gameObject->GetId());
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesFolderName() + "stubbyarrows.model");
			FRAMEWORK->GetLogger()->dbglog("\nStubby Arrows id: %d", gameObject->GetId());

			Transform* transform = gameObject->GetTransform();
			transform->Scale(0.6f, 0.6f, 0.6f);
			transform->SetPosition(wavybox->GetTransform()->GetPositionWorld());
		}
		{
			GameObject* camera = new GameObject(ENGINE->GetSceneGraph3D());
			ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(camera->GetId());
			Camera* cameraComponent = camera->AddComponent<Camera>();
			cameraComponent->SetCameraType(CAMERA_TYPE_PERSPECTIVE);
			camera->GetTransform()->SetPosition(4.0f, 4.0f, 4.0f);
			// camera->GetTransform()->SetOrientation(-45.0f, camera->GetTransform()->GetUp());
			// camera->GetTransform()->Rotate(45.0f inRadians, camera->GetTransform()->GetLeft());
			camera->GetTransform()->LookAt(0.0f, 0.0f, 0.0f);
			ENGINE->GetSceneGraph3D()->SetMainCameraId(camera->GetId());
		}
		{
			GameObject* dlight = new GameObject(ENGINE->GetSceneGraph3D());
			ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(dlight->GetId());
			DirectionalLight* dlightComponent = dlight->AddComponent<DirectionalLight>();
			dlight->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
			dlight->GetTransform()->LookAt(0.5f, 10.0f, 0.5f);
			ENGINE->GetSceneGraph3D()->SetMainDirectionalLightId(dlight->GetId());
			//
			dlightComponent->SetAmbientColor(0.15f, 0.15f, 0.3f, 1.0f);
			dlightComponent->SetDiffuseColor(0.5f, 0.5f, 0.55f, 1.0f);
		}
		{
			GameObject* gameObject = new GameObject(ENGINE->GetSceneGraph3D());
			ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(gameObject->GetId());
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesFolderName() + "TexturedCube.model");

			Transform* transform = gameObject->GetTransform();
			transform->Scale(0.6f);
			transform->SetPosition(25.0f, 0.0f, -10.0f);
			transform->SetOrientation(PI / 2.0f, YAXIS);

			glm::vec3 currentPosition = transform->GetPosition();
			glm::vec3 finalPosition   = currentPosition + glm::vec3(10.0f, 0.0f, 0.0f);

			glm::quat currentOrientation = transform->GetOrientation();
			glm::quat finalOrientation   = glm::angleAxis(PI / 4.0f, YAXIS);

			glm::vec3 currentScale = transform->GetScale();
			glm::vec3 finalScale   = currentScale;

#if 0
			ENGINE->GetTween()->TweenPosition(gameObject->GetId(),
											  currentPosition, finalPosition,
											  10.0f);
#endif
#if 1
			ENGINE->GetTween()->TweenTransform(gameObject->GetId(),
											   currentPosition, finalPosition,
											   currentOrientation, finalOrientation,
											   currentScale, finalScale,
											   10.0f,
											   true);
#endif
		}
		{
#if 0
			GameObject* gameObject = new GameObject(ENGINE->GetSceneGraph3D());
			ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(gameObject->GetId());
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesFolderName() + "bone.model");
			parent = gameObject;

			Transform* transform = parent->GetTransform();
			transform->Scale(0.2f);
#endif
		}
		{
#if 0
			GameObject* noiseMaker = new GameObject(ENGINE->GetSceneGraph3D());
			ENGINE->GetSceneGraph3D()->GetRootGameObject()->AddChild(noiseMaker->GetId());
			AudioSource* source = noiseMaker->AddComponent<AudioSource>();
			source->SetAudioClip(FRAMEWORK->GetFileSystemUtils()->GetDeviceAudioResourcesFolderName() + "lowAlert(hisfixed).wav");
			source->Play();
#endif
		}


		FRAMEWORK->GetLogger()->dbglog("\nDone loading game objects for the scene");
	}

}
