#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Lighting/ShadowMap.h"
#include "Vajra/Engine/Lighting/ShadowMapUpdateListener.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

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
	Camera* depthCamera = depthCameraObject->AddComponent<Camera>();
	this->depthCameraId = depthCameraObject->GetId();

	this->adjustDepthCamera();
}

void ShadowMap::adjustDepthCamera() {
	GameObject* depthCameraObject = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->depthCameraId);
	if (depthCameraObject == nullptr) {
		this->createDepthCamera();
	}

	VERIFY(depthCameraObject != nullptr, "Depth camera object present");
	Camera* depthCamera = depthCameraObject->GetComponent<Camera>();
	VERIFY(depthCamera != nullptr, "Depth camera object has camera component");

	depthCameraObject->GetTransform()->SetPosition(10.0f, 0.0f, -4.0f);
	depthCameraObject->GetTransform()->SetOrientation(IDENTITY_QUATERNION);
	depthCameraObject->GetTransform()->Rotate(150.0f inRadians, YAXIS);
	depthCameraObject->GetTransform()->Rotate(-80.0f inRadians, XAXIS);
	depthCameraObject->GetTransform()->Translate(-20.0f, depthCameraObject->GetTransform()->GetForward());

	depthCamera->SetCameraType(CAMERA_TYPE_ORTHO);
	depthCamera->SetOrthoBounds(-20.0f, 20.0f, -20.0f, 20.0f, -10.5f, 100.0f);
}

void ShadowMap::mainCameraChanged() {
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

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->depthTexture);
		checkGlError("glBindTexture");
		glUniform1i(textureHandle, 2);
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
		glUniformMatrix4fv(depthBiasMvpMatrixHandle, 1, GL_FALSE, glm::value_ptr(depthBiasMvpMatrix));
	}
}

void ShadowMap::init() {
	this->createDepthCamera();

	this->AddComponent<ShadowMapUpdateListener>();
}

void ShadowMap::destroy() {
}

