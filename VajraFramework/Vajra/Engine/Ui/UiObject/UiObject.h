#ifndef UIOBJECT_H
#define UIOBJECT_H

#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Input/Definitions.h"

// Forward Declarations:
class SceneGraph;
class UiTouchHandlers;

class UiObject : public GameObject {
public:
	UiObject(SceneGraph* sceneGraph);
	virtual ~UiObject();

	std::string GetUiObjectName() { return this->uiObjectName; }
	void SetUiObjectName(std::string name_) { this->uiObjectName = name_; }

	unsigned int GetWidth()            { return this->width;           }
	unsigned int GetHeight()           { return this->height;          }
	//
	bool IsClickable()                 { return this->clickable;       }
	void SetClickable(bool clickable_, UiTouchHandlers* touchHandlers_ = nullptr);

	void OnTouchDown(Touch touch);
	void OnTouchMove(Touch touch);
	void OnTouchUp(Touch touch);
	bool IsPointWithin(float x, float y);

	void SetPosition(unsigned int x, unsigned int y);

protected:
	void setWidth(unsigned int w)      { this->width = w;              }
	void setHeight(unsigned int h)     { this->height = h;             }

private:
	void init();
	void destroy();

	std::string uiObjectName;

	unsigned int width, height;
	bool clickable;

	UiTouchHandlers* touchHandlers;
};

#endif // UIOBJECT_H
