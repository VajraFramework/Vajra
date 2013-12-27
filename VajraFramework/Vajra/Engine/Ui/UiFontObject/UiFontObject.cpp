#include "Vajra/Engine/Components/DerivedComponents/Renderer/UiFontRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Ui/UiFontObject/UiFontObject.h"

UiFontObject::UiFontObject(SceneGraph* sceneGraph) : UiObject(sceneGraph) {
	this->init();
}

UiFontObject::~UiFontObject() {
	this->destroy();
}

void UiFontObject::InitTextToDisplay(std::string text, unsigned int width, unsigned int height, UiFontType* fontType_) {
	ASSERT(this->GetComponent<Renderer>() == nullptr, "UiObject doesn't already have a Renderer on it");
	UiFontRenderer* uiRenderer = this->AddComponent<UiFontRenderer>();
	uiRenderer->initTextToDisplay(text, width, height, fontType_);
	//
	this->setWidth(width);
	this->setHeight(height);
	this->setText(text);
}

void UiFontObject::init() {
}

void UiFontObject::destroy() {
}
