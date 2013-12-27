#ifndef UIFONTOBJECT_H
#define UIFONTOBJECT_H

#include "Vajra/Engine/GameObject/GameObject.h"

// Forward Declarations:
class SceneGraph;

class UiFontObject : public GameObject {
public:
	UiFontObject(SceneGraph* sceneGraph);
	virtual ~UiFontObject();

	void InitTextToDisplay(std::string text, unsigned int width, unsigned int height, UiFontType* fontType_);

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
	void setText(std::string text)     { this->textToDisplay = text;   }

	std::string textToDisplay;
	unsigned int width, height;
	bool clickable;

};

#endif // UIFONTOBJECT_H
