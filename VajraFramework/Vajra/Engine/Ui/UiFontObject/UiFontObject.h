#ifndef UIFONTOBJECT_H
#define UIFONTOBJECT_H

#include "Vajra/Engine/Ui/UiObject/UiObject.h"

// Forward Declarations:
class SceneGraph;
class UiFontType;

class UiFontObject : public UiObject {
public:
private:
	UiFontObject(SceneGraph* sceneGraph);
	virtual ~UiFontObject();

	// TODO [Cleanup] Make this private and friended to UiElement
	void InitTextToDisplay(std::string text, unsigned int width, unsigned int height, std::string pathToFontSpecificationFile);

	void init();
	void destroy();

	void setText(std::string text)     { this->textToDisplay = text;   }

	std::string getPathToFontFile() { return this->pathToFontFile; }

	std::string textToDisplay;
	std::string pathToFontFile;

	friend class UiElement;
};

#endif // UIFONTOBJECT_H
