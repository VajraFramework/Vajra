#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/Mesh.h"
#include "Vajra/Engine/Components/DerivedComponents/MeshRenderer/MeshRenderer.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Placeholder/Tesserakonteres.h"
#include "Vajra/Framework/Logging/Logger.h"

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
		{
			GameObject* gameObject = new GameObject();
			MeshRenderer* meshRenderer = gameObject->AddComponent<MeshRenderer>();
			Mesh* mesh = new Mesh();
			Tesserakonteres::initModels(mesh);
			meshRenderer->InitMesh(mesh);
			ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(gameObject->GetId());
		}
		{
			GameObject* camera = new GameObject();
			/* Camera* cameraComponent = */ camera->AddComponent<Camera>();
			camera->GetTransform()->SetPosition(2.0f, 2.0f, 2.0f);
			ENGINE->GetSceneGraph()->GetRootGameObject()->AddChild(camera->GetId());
			ENGINE->GetSceneGraph()->SetMainCameraId(camera->GetId());
		}
	}

	// Stuff to initialize Mesh with:
	std::vector<glm::vec3> meshPositions;
	std::vector<glm::vec3> meshNormals;
	std::vector<glm::vec2> meshTextureCoords;
	std::vector<unsigned int> meshIndices;
	Mesh *mesh;
	//
	void initModels(Mesh* mesh) {
		meshPositions.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
		meshPositions.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
		meshPositions.push_back(glm::vec3(0.5f, 0.5f, 0.0f));
		meshPositions.push_back(glm::vec3(-0.5f, 0.5f, 0.0f));
	    //
		for (int i = 0; i < 4; ++i) {
			meshNormals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	    //
		meshTextureCoords.push_back(glm::vec2(0.0f, 0.0f));
		meshTextureCoords.push_back(glm::vec2(1.0f, 0.0f));
		meshTextureCoords.push_back(glm::vec2(1.0f, 1.0f));
		meshTextureCoords.push_back(glm::vec2(0.0f, 1.0f));

		unsigned int indices_data[] = {
			0, 1, 2,
			0, 2, 3
		};
		for (int i = 0; i < 6; ++i) {
			meshIndices.push_back(indices_data[i]);
		}

		mesh->InitVerticesData(meshPositions, meshNormals, meshTextureCoords);
		mesh->InitIndicesData(meshIndices);

		mesh->MakeVBOs();
	}

}
