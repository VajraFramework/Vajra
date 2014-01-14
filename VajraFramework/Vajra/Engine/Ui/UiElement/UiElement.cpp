#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"

UiElement::UiElement(SceneGraph* sceneGraph) : UiObject(sceneGraph) {
	this->init();
}

UiElement::~UiElement() {
	this->destroy();
}

void UiElement::InitSprite(unsigned int width, unsigned int height, std::string shaderName_, std::string pathToTexture) {
	this->addChildSpriteObject();

	this->setWidth(width);
	this->setHeight(height);
	//
	this->childSpriteObjectRef->InitSprite(width, height, shaderName_, pathToTexture);
}

void UiElement::InitSprite(unsigned int width, unsigned int height, std::string shaderName_, glm::vec4 color) {
	this->addChildSpriteObject();

	this->setWidth(width);
	this->setHeight(height);
	//
	this->childSpriteObjectRef->InitSprite(width, height, shaderName_, color);
}

void UiElement::InitTextToDisplay(std::string text, unsigned int width, unsigned int height, std::string pathToFontSpecificationFile) {
	this->addChildFontObject();

	this->setWidth(width);
	this->setHeight(height);

	this->childFontObjectRef->InitTextToDisplay(text, width, height, pathToFontSpecificationFile);

	// TODO [Hack] Change this to use a font size concept instead
	this->childFontObjectRef->GetTransform()->Scale(30.0f);
}

void UiElement::init() {
	this->childSpriteObjectRef = nullptr;
	this->childFontObjectRef   = nullptr;
}

void UiElement::destroy() {
}

void UiElement::addChildSpriteObject() {
	ASSERT(this->childSpriteObjectRef == nullptr, "Setting sprite for the first time");
	UiSpriteObject* uiSpriteObject = new UiSpriteObject(this->GetParentSceneGraph());
	this->AddChild(uiSpriteObject->GetId());
	//
	this->childSpriteObjectRef = uiSpriteObject;
}

void UiElement::addChildFontObject() {
	ASSERT(this->childFontObjectRef == nullptr, "Setting font for the first time");
	UiFontObject* uiFontObject = new UiFontObject(this->GetParentSceneGraph());
	this->AddChild(uiFontObject->GetId());
	//
	this->childFontObjectRef = uiFontObject;
}
