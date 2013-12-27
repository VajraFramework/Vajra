#ifndef UISPRITEOBJECT_H
#define UISPRITEOBJECT_H

#include "Vajra/Engine/Ui/UiObject/UiObject.h"

// Forward Declarations:
class SceneGraph;

class UiSpriteObject : public UiObject {
public:
	UiSpriteObject(SceneGraph* sceneGraph);
	virtual ~UiSpriteObject();

	void InitSprite(unsigned int width, unsigned int height, std::string shaderName_, std::string pathToTexture = "");
	void InitSprite(unsigned int width, unsigned int height, std::string shaderName_, glm::vec4 color);

private:
	void init();
	void destroy();
};

#endif // UISPRITEOBJECT_H
