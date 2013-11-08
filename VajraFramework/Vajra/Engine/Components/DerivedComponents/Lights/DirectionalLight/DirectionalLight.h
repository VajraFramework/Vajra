#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Vajra/Common/Components/Component.h"

#include "Libraries/glm/glm.hpp"

// Forward Declarations:
class Object;

class DirectionalLight : public Component {
public:
	DirectionalLight();
	DirectionalLight(Object* object_);
	~DirectionalLight();

	static inline unsigned int GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(Message* message);

	void WriteLightPropertiesToShader();

	void SetAmbientColor (float r, float g, float b, float a) { this->ambientColor  = glm::vec4(r, g, b, a); }
	void SetDiffuseColor (float r, float g, float b, float a) { this->diffuseColor  = glm::vec4(r, g, b, a); }
	void SetSpecularColor(float r, float g, float b, float a) { this->specularColor = glm::vec4(r, g, b, a); }

private:
	void init();
	void destroy();

	glm::vec4 ambientColor;
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;

	static unsigned int componentTypeId;
};

#endif // DIRECTIONAL_LIGHT_H
