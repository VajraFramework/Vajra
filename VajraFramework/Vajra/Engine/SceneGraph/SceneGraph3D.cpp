#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/RenderScene/RenderScene.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/SceneGraph/RenderLists.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"
#include "Vajra/Engine/Lighting/ShadowMap.h"

SceneGraph3D::SceneGraph3D() : SceneGraph() {
}

SceneGraph3D::~SceneGraph3D() {
	this->destroy();
}

// TODO [Cleanup] Cache the mainDirectionalLight, maybe
DirectionalLight* SceneGraph3D::GetMainDirectionalLight() {
	if (this->mainDirectionalLightId != OBJECT_ID_INVALID) {
		GameObject *directionalLight = this->GetGameObjectById(this->mainDirectionalLightId);
		if (directionalLight != nullptr) {
			return directionalLight->GetComponent<DirectionalLight>();
		}
	}
	return nullptr;
}

void SceneGraph3D::UnsetMainDirectionalLightId() {
	this->mainDirectionalLightId = OBJECT_ID_INVALID;
}

void SceneGraph3D::SetMainDirectionalLightId(ObjectIdType id) {
	GameObject *directionalLight = this->GetGameObjectById(id);
	ASSERT(directionalLight != nullptr, "New directionalLight object not null");
	ASSERT(directionalLight->GetComponent<DirectionalLight>() != nullptr, "New directionalLight object has a DirectionalLight component");
	this->mainDirectionalLightId = id;
}

void SceneGraph3D::AddAdditionalLightId(ObjectIdType id) {
	GameObject *directionalLight = this->GetGameObjectById(id);
	ASSERT(directionalLight != nullptr, "New additional directionalLight object not null");
	ASSERT(directionalLight->GetComponent<DirectionalLight>() != nullptr, "New additional directionalLight object has a DirectionalLight component");
	this->additionalLightIds.push_back(id);
}

void SceneGraph3D::RemoveAdditionalLightId(ObjectIdType id) {
	auto it = std::find(this->additionalLightIds.begin(), this->additionalLightIds.end(), id);
	if (it != this->additionalLightIds.end()) {
		this->additionalLightIds.erase(it);
	}
}

void SceneGraph3D::update() {
	// TODO [Implement] Figure out if we need to put anything here
#if 0
	// Rotating main light:
	if (this->GetMainDirectionalLight() != nullptr) {
		GameObject* dlight = this->GetGameObjectById(this->GetMainDirectionalLight()->GetObject()->GetId());
		if (dlight != nullptr) {
			dlight->GetTransform()->Rotate(10.0f * ENGINE->GetTimer()->GetDeltaFrameTime() inRadians, YAXIS);

			DebugDraw::DrawArrow(dlight->GetTransform()->GetPosition(), dlight->GetTransform()->GetForward(), 2.0f);

			Camera* depthCamera = ENGINE->GetShadowMap()->GetDepthCamera();
			if (depthCamera != nullptr) {
				GameObject* depthCameraObject = (GameObject*)depthCamera->GetObject();
				DebugDraw::DrawArrow(depthCameraObject->GetTransform()->GetPosition(), depthCameraObject->GetTransform()->GetForward(), 2.0f);
			}
		}
	}
#endif
}

void SceneGraph3D::draw() {

	GameObject* mainCamera = nullptr;
	if (this->mainCameraId != OBJECT_ID_INVALID) {
		mainCamera = this->GetGameObjectById(this->mainCameraId);
	}
	if (mainCamera == nullptr) {
		FRAMEWORK->GetLogger()->dbglog("Warning: There is no camera in the SceneGraph3D");
		return;
	}
	//
	GameObject* mainDirectionalLight = nullptr;
	if (this->mainDirectionalLightId != OBJECT_ID_INVALID) {
		mainDirectionalLight = this->GetGameObjectById(this->mainDirectionalLightId);
	}
	if (mainDirectionalLight == nullptr) {
		FRAMEWORK->GetLogger()->dbglog("Warning: There is no light in the SceneGraph3D");
		return;
	}

	std::vector<DirectionalLight*> additionalLights_components;
	for (ObjectIdType additionalLightId : this->additionalLightIds) {
		if (additionalLightId != OBJECT_ID_INVALID) {
			GameObject* additionalLightObject = this->GetGameObjectById(additionalLightId);
			if (additionalLightObject != nullptr) {
				DirectionalLight* additionalLight_component = additionalLightObject->GetComponent<DirectionalLight>();
				additionalLights_components.push_back(additionalLight_component);
			}
		}
	}

	Camera* cameraComponent = mainCamera->GetComponent<Camera>();
	VERIFY(cameraComponent != nullptr, "mainCamera has Camera component");
	DirectionalLight* directionalLightComponent = mainDirectionalLight->GetComponent<DirectionalLight>();
	VERIFY(directionalLightComponent != nullptr, "mainDirectionalLight has DirectionalLight component");

	RenderScene::RenderScene(this->renderLists, cameraComponent, directionalLightComponent, additionalLights_components);
}

void SceneGraph3D::init() {
	SceneGraph::init();
	//
	this->mainCameraId = OBJECT_ID_INVALID;
	this->mainDirectionalLightId = OBJECT_ID_INVALID;
}

void SceneGraph3D::destroy() {
	SceneGraph::destroy();
}

