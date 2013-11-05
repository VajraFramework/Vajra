#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/MeshRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Placeholder/Tesserakonteres.h"

#ifdef PLATFORM_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#ifdef PLATFORM_DESKTOP
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GL/glfw.h>
#endif // PLATFORM_DESKTOP

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
			meshRenderer->InitMesh(FRAMEWORK->GetFileSystemUtils()->GetDeviceModelResourcesPath() + "Suzanne.model");
			ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(gameObject->GetId());
			Transform* transform = gameObject->GetTransform();
			transform->Scale(0.5f, 0.5f, 0.5f);
		}
		{
			GameObject* camera = new GameObject();
			/* Camera* cameraComponent = */ camera->AddComponent<Camera>();
			camera->GetTransform()->SetPosition(2.0f, 2.0f, 2.0f);
			ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(camera->GetId());
			ENGINE->GetSceneGraph()->SetMainCameraId(camera->GetId());
		}
	}
}
