#ifndef UISPRITEOBJECT_H
#define UISPRITEOBJECT_H

#include "Vajra/Engine/GameObject/GameObject.h"

// Forward Declarations:
class SceneGraph;

class UiSpriteObject : public GameObject {
public:
	UiSpriteObject(SceneGraph* sceneGraph);
	virtual ~UiSpriteObject();

	void InitSprite(unsigned int width, unsigned int height, std::string shaderName_, std::string pathToTexture = "");
	void InitSprite(unsigned int width, unsigned int height, std::string shaderName_, glm::vec4 color);

	void SetPosition(unsigned int x, unsigned int y);

	unsigned int GetWidth()            { return this->width;           }
	unsigned int GetHeight()           { return this->height;          }
	//
	bool IsClickable()                 { return this->clickable;       }
	void SetClickable(bool clickable_) { this->clickable = clickable_; }

private:
	void init();
	void destroy();

	void setWidth(unsigned int w)      { this->width = w;              }
	void setHeight(unsigned int h)     { this->height = h;             }

	unsigned int width, height;
	bool clickable;
};

#endif // UISPRITEOBJECT_H
