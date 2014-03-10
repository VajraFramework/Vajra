#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Lights/DirectionalLight/DirectionalLight.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Framework/OpenGL/OpenGLWrapper/OpenGLWrapper.h"
#include "Vajra/Framework/OpenGL/ShaderSet/ShaderSet.h"

unsigned int DirectionalLight::componentTypeId = COMPONENT_TYPE_ID_DIRECTIONAL_LIGHT;

// TODO [Hack] This should be in sync with the number of max lights in the shaders
#define MAX_ADDIONAL_LIGHTS 3
//
static unsigned int g_numAdditionalLights = 0;

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

	case MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT: {
		// Broadcast a directional light changed event:
		MessageChunk message = ENGINE->GetMessageHub()->GetOneFreeMessage();
		message->SetMessageType(MESSAGE_TYPE_DIRECTIONAL_LIGHT_CHANGED);
		message->messageData.iv1.x = this->GetObject()->GetId();
		ENGINE->GetMessageHub()->SendMulticastMessage(message, this->GetObject()->GetId());
	} break;

	default:
		FRAMEWORK->GetLogger()->dbglog("\nDirectionalLight got unnecessary msg of type %d", messageChunk->GetMessageType());
	}
}

static Shader_variable_variablename_id_t GetDirectionHandleBasedOnLightTypeAndId(std::string lightType, unsigned int lightId) {
	if (lightType == MAIN_LIGHT_STRING) {
		return SHADER_VARIABLE_VARIABLENAME_DLight0Direction;
	} else if (lightType == ADDITIONAL_LIGHT_STRING) {
		switch (lightId) {
		case 0: return SHADER_VARIABLE_VARIABLENAME_AdditionalLight0Direction;
		case 1: return SHADER_VARIABLE_VARIABLENAME_AdditionalLight1Direction;
		case 2: return SHADER_VARIABLE_VARIABLENAME_AdditionalLight2Direction;
		default: ASSERT(0, "Bad light id (max %d)", MAX_ADDIONAL_LIGHTS);
		}
	}
	ASSERT(0, "Why are we here");
	return SHADER_VARIABLE_VARIABLENAME_invalid;
}
//
static Shader_variable_variablename_id_t GetAmbientHandleBasedOnLightTypeAndId(std::string lightType, unsigned int lightId) {
	if (lightType == MAIN_LIGHT_STRING) {
		return SHADER_VARIABLE_VARIABLENAME_DLight0AmbientColor;
	} else if (lightType == ADDITIONAL_LIGHT_STRING) {
		switch (lightId) {
		case 0: return SHADER_VARIABLE_VARIABLENAME_AdditionalLight0Ambient;
		case 1: return SHADER_VARIABLE_VARIABLENAME_AdditionalLight1Ambient;
		case 2: return SHADER_VARIABLE_VARIABLENAME_AdditionalLight2Ambient;
		default: ASSERT(0, "Bad light id (max %d)", MAX_ADDIONAL_LIGHTS);
		}
	}
	ASSERT(0, "Why are we here");
	return SHADER_VARIABLE_VARIABLENAME_invalid;
}
//
static Shader_variable_variablename_id_t GetDiffuseHandleBasedOnLightTypeAndId(std::string lightType, unsigned int lightId) {
	if (lightType == MAIN_LIGHT_STRING) {
		return SHADER_VARIABLE_VARIABLENAME_DLight0DiffuseColor;
	} else if (lightType == ADDITIONAL_LIGHT_STRING) {
		switch (lightId) {
		case 0: return SHADER_VARIABLE_VARIABLENAME_AdditionalLight0Diffuse;
		case 1: return SHADER_VARIABLE_VARIABLENAME_AdditionalLight1Diffuse;
		case 2: return SHADER_VARIABLE_VARIABLENAME_AdditionalLight2Diffuse;
		default: ASSERT(0, "Bad light id (max %d)", MAX_ADDIONAL_LIGHTS);
		}
	}
	ASSERT(0, "Why are we here");
	return SHADER_VARIABLE_VARIABLENAME_invalid;
}

void DirectionalLight::WriteLightPropertiesToShader() {
	GameObject* gameObject = (GameObject*)this->GetObject();
	glm::vec3 forward = gameObject->GetTransform()->GetForward();
	ShaderSet* currentShaderSet = FRAMEWORK->GetOpenGLWrapper()->GetCurrentShaderSet();

	Shader_variable_variablename_id_t directionHandle = GetDirectionHandleBasedOnLightTypeAndId(this->lightType, this->lightId);
	if (currentShaderSet->HasHandle(directionHandle)) {
		glUniform4f(currentShaderSet->GetHandle(directionHandle),
												forward.x, forward.y, forward.z, 0.0f);
	}
	//
	Shader_variable_variablename_id_t ambientHandle = GetAmbientHandleBasedOnLightTypeAndId(this->lightType, this->lightId);
	if (currentShaderSet->HasHandle(ambientHandle)) {
		glUniform4f(currentShaderSet->GetHandle(ambientHandle),
				this->ambientColor.r * this->intensity, this->ambientColor.g * this->intensity, this->ambientColor.b * this->intensity, this->ambientColor.a * this->intensity);
	}
	Shader_variable_variablename_id_t diffuseHandle = GetDiffuseHandleBasedOnLightTypeAndId(this->lightType, this->lightId);
	if (currentShaderSet->HasHandle(diffuseHandle)) {
		glUniform4f(currentShaderSet->GetHandle(diffuseHandle),
				this->diffuseColor.r * this->intensity, this->diffuseColor.g * this->intensity, this->diffuseColor.b * this->intensity, this->diffuseColor.a * this->intensity);
	}

	// TODO [Hack] Removing specular lights for now
}

void DirectionalLight::SetIntensity(float intensity_) {
	this->intensity = intensity_;
}

void DirectionalLight::SetAmbientColor (float r, float g, float b, float a) {
	this->ambientColor  = glm::vec4(r, g, b, a);
}

void DirectionalLight::SetAmbientColorInts(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
	this->SetAmbientColor((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f);
}

void DirectionalLight::SetDiffuseColor (float r, float g, float b, float a) {
	this->diffuseColor  = glm::vec4(r, g, b, a);
}

void DirectionalLight::SetDiffuseColorInts(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
	this->SetDiffuseColor((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f);
}

void DirectionalLight::SetSpecularColor(float r, float g, float b, float a) {
	this->specularColor = glm::vec4(r, g, b, a);
}

void DirectionalLight::SetLightType(std::string lightType_) {
	VERIFY(lightType_ == MAIN_LIGHT_STRING || lightType_ == ADDITIONAL_LIGHT_STRING, "LightType (%s) one of \"%s\" or \"%s\"", lightType.c_str(), MAIN_LIGHT_STRING, ADDITIONAL_LIGHT_STRING);
	this->lightType = lightType_;

	if (this->lightType == MAIN_LIGHT_STRING) {
		this->lightId = 0;

		// Now that we know our type, we can register ourselves with the scene graph
		ENGINE->GetSceneGraph3D()->SetMainDirectionalLightId(this->GetObject()->GetId());

	} else if (this->lightType == ADDITIONAL_LIGHT_STRING) {
		this->lightId = g_numAdditionalLights;
		g_numAdditionalLights++;

		ASSERT(g_numAdditionalLights <= MAX_ADDIONAL_LIGHTS, "Not too many aditional lights (max %d)", MAX_ADDIONAL_LIGHTS);
		if (g_numAdditionalLights > MAX_ADDIONAL_LIGHTS) {
			FRAMEWORK->GetLogger()->dbglog("Too many additional lights: %u (max %d)", g_numAdditionalLights, MAX_ADDIONAL_LIGHTS);
			return;
		}

		// Now that we know our type, we can register ourselves with the scene graph
		ENGINE->GetSceneGraph3D()->AddAdditionalLightId(this->GetObject()->GetId());
	}
}



void DirectionalLight::init() {
	GameObject* gameObject = dynamic_cast<GameObject*>(this->GetObject());
	if (gameObject != nullptr) {
		ASSERT(typeid(gameObject) == typeid(GameObject*), "Type of Object* (%s) of id %d was %s", typeid(gameObject).name(), gameObject->GetId(), typeid(GameObject*).name());
	}

	this->intensity = 1.0f;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT, this->GetTypeId(), true);
}

void DirectionalLight::destroy() {
	if (this->lightType == ADDITIONAL_LIGHT_STRING) {
		g_numAdditionalLights--;
	}

	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

