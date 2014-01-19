#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Vajra/Common/Components/Component.h"

#include "Libraries/glm/glm.hpp"

// Forward Declarations:
class Object;

//[[COMPONENT]]//
class DirectionalLight : public Component {
public:
	DirectionalLight();
	DirectionalLight(Object* object_);
	~DirectionalLight();

	static inline unsigned int GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	void WriteLightPropertiesToShader();

	//[[PROPERTY]]//
	void SetAmbientColor (float r, float g, float b, float a);
	//[[PROPERTY]]//
	void SetDiffuseColor (float r, float g, float b, float a);
	//[[PROPERTY]]//
	void SetSpecularColor(float r, float g, float b, float a);

private:
	void init();
	void destroy();

	glm::vec4 ambientColor;
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;

	static unsigned int componentTypeId;
};

////////////////////////////////////////////////////////////////////////////////
// Inline Functions:

void DirectionalLight::SetAmbientColor (float r, float g, float b, float a) {
	this->ambientColor  = glm::vec4(r, g, b, a);
}

void DirectionalLight::SetDiffuseColor (float r, float g, float b, float a) {
	this->diffuseColor  = glm::vec4(r, g, b, a);
}

void DirectionalLight::SetSpecularColor(float r, float g, float b, float a) {
	this->specularColor = glm::vec4(r, g, b, a);
}

#endif // DIRECTIONAL_LIGHT_H
