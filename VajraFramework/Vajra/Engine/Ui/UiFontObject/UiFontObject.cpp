#include "Vajra/Engine/Components/DerivedComponents/Renderer/UiFontRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Ui/UiFontObject/UiFontObject.h"

UiFontObject::UiFontObject(SceneGraph* sceneGraph) : UiObject(sceneGraph) {
	this->init();
}

UiFontObject::~UiFontObject() {
	this->destroy();
}

void UiFontObject::InitTextToDisplay(std::string text, unsigned int width, unsigned int height, std::string pathToFontSpecificationFile) {
	UiFontRenderer* uiRenderer = this->GetComponent<UiFontRenderer>();
	if(uiRenderer == nullptr) {
		uiRenderer = this->AddComponent<UiFontRenderer>();
	}
	uiRenderer->initTextToDisplay(text, width, height, pathToFontSpecificationFile);
	uiRenderer->SetHasTransperancy(true);
	//
	this->setWidth(width);
	this->setHeight(height);
	this->setText(text);
	//
	this->pathToFontFile = pathToFontSpecificationFile;
}

void UiFontObject::init() {
}

void UiFontObject::destroy() {
}
