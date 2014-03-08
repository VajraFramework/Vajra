#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Lighting/ShadowMap.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

ShadowMap::ShadowMap() {
	// Don't call init here
}

ShadowMap::~ShadowMap() {
	this->destroy();
}

void ShadowMap::Draw() {
	
	return;

	Camera* camera = this->depthCamera;

	ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();
	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_depthTextureSampler)) {
		GLint textureHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_depthTextureSampler);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, this->depthTexture);
		checkGlError("glBindTexture");
		glUniform1i(textureHandle, 2);
	}

	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_depthBiasMVPMatrix)) {
		GameObject* cameraObject = (GameObject*)camera->GetObject();
		ASSERT(cameraObject->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");

		glm::mat4 depthProjectionMatrix = camera->GetProjMatrix();
		glm::mat4 depthViewMatrix = camera->GetViewMatrix();
		glm::mat4 depthModelMatrix = glm::mat4(1.0);

		glm::mat4 depthMvpMatrix = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

		float bias = 0.5f;
		glm::mat4 biasMatrix(
					bias, 0.0, 0.0, 0.0,
					0.0, bias, 0.0, 0.0,
					0.0, 0.0, bias, 0.0,
					bias, bias, bias, 1.0
				);


		glm::mat4 depthBiasMvpMatrix = biasMatrix * depthMvpMatrix;

		GLint depthBiasMvpMatrixHandle = currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_depthBiasMVPMatrix);
		glUniformMatrix4fv(depthBiasMvpMatrixHandle, 1, GL_FALSE, glm::value_ptr(depthBiasMvpMatrix));
	}
}

void ShadowMap::init() {
}

void ShadowMap::destroy() {
}
