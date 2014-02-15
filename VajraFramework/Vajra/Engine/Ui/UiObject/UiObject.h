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

	unsigned int GetWidth()            { return this->width;           }
	unsigned int GetHeight()           { return this->height;          }
	int          GetZOrder()           { return this->zorder;          }
	//
	virtual void SetVisible(bool visible_);
	bool IsClickable()                 { return this->clickable;       }
	void SetClickable(bool clickable_);
	void SetTouchHandlers(UiTouchHandlers* touchHandlers_ = nullptr);

	void OnTouchDown(Touch touch);
	void OnTouchMove(Touch touch);
	void OnTouchUp(Touch touch);
	bool IsPointWithin(float x, float y);

	void SetPosition(unsigned int x, unsigned int y);
	void SetZOrder(int zorder_);

protected:
	void setWidth(unsigned int w)      { this->width = w;              }
	void setHeight(unsigned int h)     { this->height = h;             }

private:
	void init();
	void destroy();

	void registerWithTouchHandlers();
	void unregisterWithTouchHandlers();

	unsigned int width, height;
	int zorder;

	bool clickable;

	UiTouchHandlers* touchHandlers;
};

#endif // UIOBJECT_H
