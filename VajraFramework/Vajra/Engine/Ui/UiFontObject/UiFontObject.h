#ifndef UIFONTOBJECT_H
#define UIFONTOBJECT_H

#include "Vajra/Engine/Ui/UiObject/UiObject.h"

// Forward Declarations:
class SceneGraph;

class UiFontObject : public UiObject {
public:
	UiFontObject(SceneGraph* sceneGraph);
	virtual ~UiFontObject();

	void InitTextToDisplay(std::string text, unsigned int width, unsigned int height, UiFontType* fontType_);

private:
	void init();
	void destroy();

	void setText(std::string text)     { this->textToDisplay = text;   }

	std::string textToDisplay;
};

#endif // UIFONTOBJECT_H
