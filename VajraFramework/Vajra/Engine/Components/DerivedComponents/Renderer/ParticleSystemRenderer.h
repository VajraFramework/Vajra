#ifndef PARTICLESYSTEMRENDERER_H
#define PARTICLESYSTEMRENDERER_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/Renderer.h"

#include <memory>
#include <string>
#include <vector>

// Forward Declarations:
class Object;
class Mesh;
class ParticleSystem;

//[[COMPONENT]]//
class ParticleSystemRenderer : public Renderer {
public:
	ParticleSystemRenderer();
	ParticleSystemRenderer(Object* object_);
	virtual ~ParticleSystemRenderer();

	static inline ComponentIdType GetTypeId() { return Renderer::GetTypeId(); }

	inline std::string GetShaderName() { return this->shaderName; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	// @Override
	virtual void Draw();

	inline glm::vec4 GetDiffuseColor () { return this->diffuseColor;  }

private:
	void init();
	void destroy();

	inline void setDiffuseColor (glm::vec4 color) { this->diffuseColor  = color; }

	// Utility Functions:
	void initializeRendererStructures();
	void updateVBOs();

	GLuint vboPositions;
	GLuint vboPointSizes;

	glm::vec4 diffuseColor;

	ParticleSystem* particleSystemRef;

	std::string shaderName;
};

#endif // PARTICLESYSTEMRENDERER_H
