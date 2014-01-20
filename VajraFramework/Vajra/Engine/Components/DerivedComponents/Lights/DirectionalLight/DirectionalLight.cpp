#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

unsigned int DirectionalLight::componentTypeId = COMPONENT_TYPE_ID_DIRECTIONAL_LIGHT;

DirectionalLight::DirectionalLight() : Component() {
	this->init();
}

DirectionalLight::DirectionalLight(Object* object_) : Component(object_) {
	this->init();
}

DirectionalLight::~DirectionalLight() {
	this->destroy();
}

void DirectionalLight::HandleMessage(MessageChunk messageChunk) {
	switch (messageChunk->GetMessageType()) {

	default:
		FRAMEWORK->GetLogger()->dbglog("\nDirectionalLight got unnecessary msg of type %d", messageChunk->GetMessageType());
	}
}

void DirectionalLight::WriteLightPropertiesToShader() {
	GameObject* gameObject = (GameObject*)this->GetObject();
	glm::vec3 forward = gameObject->GetTransform()->GetForward();
	ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_DLight0Direction)) {
		glUniform4f(currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_DLight0Direction),
												forward.x, forward.y, forward.z, 0.0f);
	}

	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_DLight0AmbientColor)) {
		glUniform4f(currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_DLight0AmbientColor),
				this->ambientColor.r, this->ambientColor.g, this->ambientColor.b, this->ambientColor.a);
	}
	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_DLight0DiffuseColor)) {
		glUniform4f(currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_DLight0DiffuseColor),
				this->diffuseColor.r, this->diffuseColor.g, this->diffuseColor.b, this->diffuseColor.a);
	}
	if (currentShaderSet->HasHandle(SHADER_VARIABLE_VARIABLENAME_DLight0SpecularColor)) {
		glUniform4f(currentShaderSet->GetHandle(SHADER_VARIABLE_VARIABLENAME_DLight0SpecularColor),
				this->specularColor.r, this->specularColor.g, this->specularColor.b, this->specularColor.a);
	}
}

void DirectionalLight::SetAmbientColor (float r, float g, float b, float a) {
	this->ambientColor  = glm::vec4(r, g, b, a);
}

void DirectionalLight::SetDiffuseColor (float r, float g, float b, float a) {
	this->diffuseColor  = glm::vec4(r, g, b, a);
}

void DirectionalLight::SetSpecularColor(float r, float g, float b, float a) {
	this->specularColor = glm::vec4(r, g, b, a);
}

void DirectionalLight::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	if (gameObject != nullptr) {
		this->WriteLightPropertiesToShader();
	}
}

void DirectionalLight::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
