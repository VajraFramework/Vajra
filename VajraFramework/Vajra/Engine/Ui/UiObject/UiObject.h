#ifndef UIOBJECT_H
#define UIOBJECT_H

#include "Vajra/Engine/GameObject/GameObject.h"

// Forward Declarations:
class SceneGraph;

class UiObject : public GameObject {
public:
	UiObject(SceneGraph* sceneGraph);
	virtual ~UiObject();

	unsigned int GetWidth()            { return this->width;           }
	unsigned int GetHeight()           { return this->height;          }
	//
	bool IsClickable()                 { return this->clickable;       }
	void SetClickable(bool clickable_) { this->clickable = clickable_; }

	void SetPosition(unsigned int x, unsigned int y);

protected:
	void setWidth(unsigned int w)      { this->width = w;              }
	void setHeight(unsigned int h)     { this->height = h;             }

private:
	void init();
	void destroy();

	unsigned int width, height;
	bool clickable;
};

#endif // UIOBJECT_H
