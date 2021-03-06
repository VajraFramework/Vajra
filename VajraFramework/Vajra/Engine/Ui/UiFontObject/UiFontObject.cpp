#include "Vajra/Engine/Components/DerivedComponents/Renderer/UiFontRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Ui/UiFontObject/UiFontObject.h"

UiFontObject::UiFontObject(SceneGraph* sceneGraph) : UiObject(sceneGraph) {
	this->init();
}

UiFontObject::~UiFontObject() {
	this->destroy();
}

void UiFontObject::InitTextToDisplay(std::string text, unsigned int width, unsigned int height, std::string pathToFontSpecificationFile, float fontSize, UiFontAlignment_type fontAlignment) {
	UiFontRenderer* uiRenderer = this->GetComponent<UiFontRenderer>();
	if(uiRenderer == nullptr) {
		uiRenderer = this->AddComponent<UiFontRenderer>();
	}
	uiRenderer->initTextToDisplay(text, width, height, pathToFontSpecificationFile, fontSize, fontAlignment);
	uiRenderer->SetHasTransperancy(true);
	//
	this->setWidth ((unsigned int)std::max((int)width,  (int)uiRenderer->GetWidth ()));
	this->setHeight((unsigned int)std::max((int)height, (int)uiRenderer->GetHeight()));
	this->textToDisplay = text;
	//
	this->pathToFontFile = pathToFontSpecificationFile;

}

void UiFontObject::ChangeText(std::string text) {
	UiFontRenderer* uiRenderer = this->GetComponent<UiFontRenderer>();
	VERIFY(uiRenderer != nullptr, "Looks like ChangeText() was called without ever calling InitTextToDisplay()");

	uiRenderer->changeText(text);

	this->textToDisplay = text;
}

void UiFontObject::SetFontColor(glm::vec4 color) {
	UiFontRenderer* uiRenderer = this->GetComponent<UiFontRenderer>();
	if (uiRenderer != nullptr) {
		uiRenderer->setDiffuseColor(color);
	}
}

void UiFontObject::init() {
}

void UiFontObject::destroy() {
}

