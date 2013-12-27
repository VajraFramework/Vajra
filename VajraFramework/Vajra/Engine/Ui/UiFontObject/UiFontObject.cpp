#include "Vajra/Engine/Components/DerivedComponents/Renderer/UiFontRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Ui/UiFontObject/UiFontObject.h"

UiFontObject::UiFontObject(SceneGraph* sceneGraph) : GameObject(sceneGraph) {
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

void UiFontObject::SetPosition(unsigned int x, unsigned int y) {
	// TODO [Implement] Implement zorder
	this->GetTransform()->SetPosition(x, -1.0f * y, 0.0f);
}

void UiFontObject::init() {
	this->width  = 0;
	this->height = 0;
	//
	this->clickable = false;
}

void UiFontObject::destroy() {
}
