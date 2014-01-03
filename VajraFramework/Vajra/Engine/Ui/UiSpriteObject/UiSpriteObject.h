#ifndef UISPRITEOBJECT_H
#define UISPRITEOBJECT_H

#include "Vajra/Engine/Ui/UiObject/UiObject.h"

// Forward Declarations:
class SceneGraph;

class UiSpriteObject : public UiObject {
public:
private:
	UiSpriteObject(SceneGraph* sceneGraph);
	virtual ~UiSpriteObject();

	// TODO [Cleanup] Make this private and friended to UiElement
	void InitSprite(unsigned int width, unsigned int height, std::string shaderName_, std::string pathToTexture = "");
	// TODO [Cleanup] Make this private and friended to UiElement
	void InitSprite(unsigned int width, unsigned int height, std::string shaderName_, glm::vec4 color);

	void init();
	void destroy();

	friend class UiElement;
};

#endif // UISPRITEOBJECT_H
