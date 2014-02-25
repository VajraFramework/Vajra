#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Engine/Ui/UiFontObject/UiFontObject.h"
#include "Vajra/Engine/Ui/UiSpriteObject/UiSpriteObject.h"

#include <string>
#include <vector>

// Forward Declarations:
class SceneGraph;
class UiFontObject;
class UiSpriteObject;


class UiElement : public UiObject {
public:
	UiElement(SceneGraph* sceneGraph);
	virtual ~UiElement();

	unsigned int GetSpriteTextureIndex();
	void SetSpriteTextureIndex(unsigned int textureIndex);

	void InitSprite(unsigned int width, unsigned int height, std::string shaderName_, std::vector<std::string> pathsToTextures, bool hasTransperancy);
	void InitSprite(unsigned int width, unsigned int height, std::string shaderName_, glm::vec4 color);

	void InitTextToDisplay(std::string text, unsigned int width, unsigned int height, std::string pathToFontSpecificationFile, float fontSize);

private:
	void init();
	void destroy();

	// Utility Functions:
	void addChildSpriteObject();
	void addChildFontObject();

	float loadTime;
	
	UiSpriteObject* childSpriteObjectRef;
	UiFontObject*   childFontObjectRef;
};

#endif // UIELEMENT_H
