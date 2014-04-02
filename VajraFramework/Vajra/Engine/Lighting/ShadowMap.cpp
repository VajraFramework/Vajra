#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Lighting/ShadowMap.h"
#include "Vajra/Engine/Lighting/ShadowMapUpdateListener.h"
#include "Vajra/Engine/RenderScene/RenderScene.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"
#include "Vajra/Framework/Settings/Settings.h"

ShadowMap::ShadowMap() {
	// Don't call init here
}

ShadowMap::~ShadowMap() {
	this->destroy();
}

Camera* ShadowMap::GetDepthCamera() {
	GameObject* cameraObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->depthCameraId);
	if (cameraObject == nullptr) {
		this->createDepthCamera();
		cameraObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->depthCameraId);
	}
	VERIFY(cameraObject != nullptr, "Depth camera object present");
	Camera* camera = cameraObject->GetComponent<Camera>();
	VERIFY(camera != nullptr, "Depth camera object has camera component");

	return camera;
}

void ShadowMap::createDepthCamera() {
	GameObject* depthCameraObject = new GameObject(ENGINE->GetSceneGraph3D());
	/* Camera* depthCamera = */ depthCameraObject->AddComponent<Camera>();
	this->depthCameraId = depthCameraObject->GetId();

	this->adjustDepthCamera();
}

#define DISTANCE_TO_PULL_DEPTH_CAMERA_BACK 30.0f

void ShadowMap::adjustDepthCamera() {
	GameObject* depthCameraObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->depthCameraId);
	if (depthCameraObject == nullptr) {
		this->createDepthCamera();
	}

	VERIFY(depthCameraObject != nullptr, "Depth camera object present");
	Camera* depthCamera = depthCameraObject->GetComponent<Camera>();
	VERIFY(depthCamera != nullptr, "Depth camera object has camera component");

	{
		Camera* mainCamera = ENGINE->GetSceneGraph3D()->GetMainCamera();
		if (mainCamera != nullptr) {

			// TODO [Implement] Ensure type safety here
			glm::vec3 mainCameraPosition = ((GameObject*)mainCamera->GetObject())->GetTransform()->GetPositionWorld();
			// TODO [Hack] Assuming game plays out at y = 0.0f, fix it by adding a "point of focus/interest" to camera:
			depthCameraObject->GetTransform()->SetPosition(mainCameraPosition.x, 0.0f, mainCameraPosition.z);
		}
	}

	{
		DirectionalLight* mainLight = ENGINE->GetSceneGraph3D()->GetMainDirectionalLight();
		if (mainLight != nullptr) {
			// TODO [Implement] Ensure type safety here
			glm::quat mainLightOrientation = ((GameObject*)mainLight->GetObject())->GetTransform()->GetOrientationWorld();
			depthCameraObject->GetTransform()->SetOrientation(mainLightOrientation);

		}
	}

	depthCameraObject->GetTransform()->Translate(-DISTANCE_TO_PULL_DEPTH_CAMERA_BACK, depthCameraObject->GetTransform()->GetForward());

	depthCamera->SetCameraType(CAMERA_TYPE_ORTHO);
	depthCamera->SetOrthoBounds(this->ortho_bounds_left, this->ortho_bounds_right, this->ortho_bounds_bottom, this->ortho_bounds_top, this->ortho_bounds_near, this->ortho_bounds_far);
}

void ShadowMap::mainCameraChanged() {

#ifdef DRAWING_DEPTH_BUFFER_CONTENTS
	// TODO [Hack] Prevent infinite loop when using the depth camera as the main camera (for debug purposes, to look at the depth map)
	static bool once = false;
	if (once) {
		return;
	}
	once = true;
#endif

	this->adjustDepthCamera();
}

void ShadowMap::mainDirectionalLightChanged() {
	this->adjustDepthCamera();
}

void ShadowMap::Draw() {
	
	Camera* depthCamera = this->GetDepthCamera();

	ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();
	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_depthTextureSampler)) {
		GLint textureHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_depthTextureSampler);

		GLCALL(glActiveTexture, GL_TEXTURE2);
		GLCALL(glBindTexture, GL_TEXTURE_2D, this->depthTexture);
		GLCALL(glUniform1i, textureHandle, 2);
	}

	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_depthBiasMVPMatrix)) {

		glm::mat4 depthProjectionMatrix = depthCamera->GetProjMatrix();
		glm::mat4 depthViewMatrix = depthCamera->GetViewMatrix();
		glm::mat4 depthModelMatrix = glm::mat4(1.0);

		glm::mat4 depthMvpMatrix = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

		glm::mat4 biasMatrix(
					0.5f, 0.0, 0.0, 0.0,
					0.0, 0.5f, 0.0, 0.0,
					0.0, 0.0, 0.5f, 0.0,
					0.5f, 0.5f, 0.5f, 1.0
				);

		glm::mat4 depthBiasMvpMatrix = biasMatrix * depthMvpMatrix;

		GLint depthBiasMvpMatrixHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_depthBiasMVPMatrix);
		GLCALL(glUniformMatrix4fv, depthBiasMvpMatrixHandle, 1, GL_FALSE, glm::value_ptr(depthBiasMvpMatrix));
	}
}

void ShadowMap::loadSettings() {
	SettingLevel_t shadowsSetting = FRAMEWORK->GetSettings()->GetSetting(SETTING_TYPE_shadows);
	switch (shadowsSetting) {
	case SETTING_LEVEL_low   : this->depthMap_width = this->depthMap_height = 1024; break;
	case SETTING_LEVEL_medium: this->depthMap_width = this->depthMap_height = 2048; break;
	case SETTING_LEVEL_high  : this->depthMap_width = this->depthMap_height = 2048; break;
	default                  : this->depthMap_width = this->depthMap_height = 2048; break;
	}
}

void ShadowMap::init() {

	this->createDepthCamera();

	this->ortho_bounds_left = -20.0f;
	this->ortho_bounds_right = 20.0f;
	this->ortho_bounds_bottom = -20.0f;
	this->ortho_bounds_top = 20.0f;
	this->ortho_bounds_near = -20.0f;
	this->ortho_bounds_far = 20.0f;

	this->depthMap_width  = 2048;
	this->depthMap_height = 2048;

	this->AddComponent<ShadowMapUpdateListener>();

	// Load global settings from the settings file:
	this->loadSettings();
}

void ShadowMap::destroy() {
}

void ShadowMap::SetOrthoBounds(float left, float right, float bottom, float top, float near, float far) {
	this->ortho_bounds_left   = left;
	this->ortho_bounds_right  = right;
	this->ortho_bounds_bottom = bottom;
	this->ortho_bounds_top    = top;
	this->ortho_bounds_near   = near;
	this->ortho_bounds_far    = far;
}

void ShadowMap::GetDepthMapResolution(unsigned int& out_width, unsigned int& out_height) {
	out_width  = this->depthMap_width;
	out_height = this->depthMap_height;
}
