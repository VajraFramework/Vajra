#ifndef UISPRITEOBJECT_H
#define UISPRITEOBJECT_H

#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"

#include <string>
#include <vector>

// Forward Declarations:
class SceneGraph;

class UiSpriteObject : public UiObject {
public:
private:
	UiSpriteObject(SceneGraph* sceneGraph);
	virtual ~UiSpriteObject();

	// TODO [Cleanup] Make this private and friended to UiElement
	void InitSprite(unsigned int width, unsigned int height, std::string shaderName_, std::vector<std::string> pathsToTextures);
	// TODO [Cleanup] Make this private and friended to UiElement
	void InitSprite(unsigned int width, unsigned int height, std::string shaderName_, glm::vec4 color);

	inline SpriteRenderer* GetSpriteRenderer();

	void init();
	void destroy();

	friend class UiElement;
};

////////////////////////////////////////////////////////////////////////////////

// Inline Functions:

SpriteRenderer* UiSpriteObject::GetSpriteRenderer() {
	return this->GetComponent<SpriteRenderer>();
}

#endif // UISPRITEOBJECT_H
