#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Vajra/Common/Components/Component.h"

#include "Libraries/glm/glm.hpp"

#define MAIN_LIGHT_STRING "main"
#define ADDITIONAL_LIGHT_STRING "additional"

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
	void SetIntensity(float intensity_);

	//[[PROPERTY]]//
	void SetAmbientColor (float r, float g, float b, float a);
	//[[PROPERTY]]//
	void SetAmbientColorInts (unsigned int r, unsigned int g, unsigned int b, unsigned int a);
	//[[PROPERTY]]//
	void SetDiffuseColor (float r, float g, float b, float a);
	//[[PROPERTY]]//
	void SetDiffuseColorInts (unsigned int r, unsigned int g, unsigned int b, unsigned int a);
	//[[PROPERTY]]//
	void SetSpecularColor(float r, float g, float b, float a);

	//[[PROPERTY]]//
	void SetLightType(std::string lightType_);

private:
	void init();
	void destroy();

	float intensity;

	glm::vec4 ambientColor;
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;

	std::string lightType;
	unsigned int lightId;

	static unsigned int componentTypeId;
};

#endif // DIRECTIONAL_LIGHT_H
