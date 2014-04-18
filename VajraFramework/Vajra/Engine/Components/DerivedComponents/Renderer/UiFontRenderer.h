#ifndef UI_FONTRENDERER_H
#define UI_FONTRENDERER_H

#include "Vajra/Engine/Ui/Definitions.h"
#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/Renderer.h"

#include <memory>
#include <string>

// Forward Declarations:
class Object;
class Mesh;
class UiFontType;

// Not exposing this as a Component that can be added via XML since it is exposed only via the UiELement which is exposed adequately by the .uiscene files
class UiFontRenderer : public Renderer {
public:
	UiFontRenderer();
	UiFontRenderer(Object* object_);
	virtual ~UiFontRenderer();

	static inline ComponentIdType GetTypeId() { return Renderer::GetTypeId(); }


	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	// @Override
	virtual void Draw();

	inline std::string GetTextToDisplay() { return this->textToDisplay; }

	inline glm::vec4 GetDiffuseColor () { return this->diffuseColor;  }

	inline void SetHasTransperancy(bool hasTransperancy_);

	inline float GetWidth () { return this->actual_width; }
	inline float GetHeight() { return this->actual_height; }

private:
	void init();
	void destroy();

	void initTextToDisplay(std::string text, unsigned int width, unsigned int height, std::string pathToFontSpecificationFile, float fontSize_, UiFontAlignment_type fontAlignment_ = UI_FONT_ALIGNMENT_left);
	inline void setDiffuseColor (glm::vec4 color) { this->diffuseColor  = color; }

	// Utility Functions:
	void makeText();
	float makeACharacter(int charIdxInAscii, int letterIdx, float woffset);
	void initVbos();

	std::string textToDisplay;

	GLuint vboPositions;
	GLuint vboTextureCoords;
	GLuint vboIndices;

	unsigned int numVertices;
	glm::vec3* vertices;
	glm::vec2* textureCoords;
	std::vector<unsigned int> indices;

	glm::vec4 diffuseColor;

	std::shared_ptr<UiFontType> fontType;

	float actual_width;
	float actual_height;
	//
	float required_width;
	float required_height;
	//
	float unscaled_width;
	float unscaled_height;

	float fontSize;
	UiFontAlignment_type fontAlignment;

	// Friended to UiObject so as not to have to expose initPlane(), etc
	friend class UiFontObject;
};

////////////////////////////////////////////////////////////////////////////////

// Inline functions:

void UiFontRenderer::SetHasTransperancy(bool hasTransperancy_) {
	Renderer::setHasTransperancy(hasTransperancy_);
}

#endif // UI_RENDERER_H
