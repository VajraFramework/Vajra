#ifndef UI_SPRITERENDERER_H
#define UI_SPRITERENDERER_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/Renderer.h"

#include <memory>
#include <string>
#include <vector>

// Forward Declarations:
class Object;
class Mesh;

enum PlaneOrigin {
	Bottom_Left,
	Bottom_Right,
	Top_Left,
	Top_Right,
	Center
};

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

	inline void setDiffuseColor (glm::vec4 color) { this->diffuseColor  = color; }
	inline glm::vec4 GetDiffuseColor () { return this->diffuseColor;  }

	inline unsigned int GetCurrentTextureIndex() { return this->currentTextureIndex; }
	inline void SetCurrentTextureIndex(unsigned int textureIndex) { this->currentTextureIndex = textureIndex; }

	void initPlane(unsigned int width, unsigned int height, std::string shaderName_, std::vector<std::string> pathsToTextures, PlaneOrigin planeOrigin = Bottom_Left);

	inline void SetHasTransperancy(bool hasTransperancy_);
	
private:
	void init();
	void destroy();

	// Utility Functions:
	void initVbos();
	inline std::shared_ptr<TextureAsset> getTextureAssetByIndex(unsigned int index);
	inline unsigned int getNumberOfTextureAssets();

	GLuint vboPositions;
	GLuint vboTextureCoords;
	GLuint vboIndices;

	glm::vec3* vertices;
	glm::vec2* textureCoords;
	std::vector<unsigned int> indices;

	glm::vec4 diffuseColor;

	unsigned int currentTextureIndex;
	std::vector< std::shared_ptr<TextureAsset> > listOfTextureAssets;

	// Friended to UiObject so as not to have to expose initPlane(), etc
	friend class UiSpriteObject;
};

////////////////////////////////////////////////////////////////////////////////

// Inline Functions:

std::shared_ptr<TextureAsset> SpriteRenderer::getTextureAssetByIndex(unsigned int index) {
	VERIFY(index < this->listOfTextureAssets.size(), "Texture asset index (%d) is within bounds", index, this->listOfTextureAssets.size());
	return this->listOfTextureAssets[index];
}

unsigned int SpriteRenderer::getNumberOfTextureAssets() {
	return this->listOfTextureAssets.size();
}

void SpriteRenderer::SetHasTransperancy(bool hasTransperancy_) {
	Renderer::setHasTransperancy(hasTransperancy_);
}

#endif // UI_SPRITERENDERER_H
