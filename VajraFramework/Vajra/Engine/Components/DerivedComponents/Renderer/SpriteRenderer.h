#ifndef UI_SPRITERENDERER_H
#define UI_SPRITERENDERER_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/Renderer.h"

#include <memory>
#include <string>

// Forward Declarations:
class Object;
class Mesh;

// Not exposing this as a Component that can be added via XML since it is exposed only via the UiELement which is exposed adequately by the .uiscene files
class SpriteRenderer : public Renderer {
public:
	SpriteRenderer();
	SpriteRenderer(Object* object_);
	virtual ~SpriteRenderer();

	static inline ComponentIdType GetTypeId() { return Renderer::GetTypeId(); }


	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	// @Override
	virtual void Draw();

	inline glm::vec4 GetDiffuseColor () { return this->diffuseColor;  }

private:
	void init();
	void destroy();

	void initPlane(unsigned int width, unsigned int height, std::string shaderName_, std::string pathToTexture = "");
	inline void setDiffuseColor (glm::vec4 color) { this->diffuseColor  = color; }

	// Utility Functions:
	void initVbos();

	GLuint vboPositions;
	GLuint vboTextureCoords;
	GLuint vboIndices;

	glm::vec3* vertices;
	glm::vec2* textureCoords;
	std::vector<unsigned int> indices;

	glm::vec4 diffuseColor;

	std::shared_ptr<TextureAsset> textureAsset;

	// Friended to UiObject so as not to have to expose initPlane(), etc
	friend class UiSpriteObject;
};

#endif // UI_SPRITERENDERER_H
