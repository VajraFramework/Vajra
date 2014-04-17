#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Engine/Ui/UiElement/UiElement.h"

UiElement::UiElement(SceneGraph* sceneGraph) : UiObject(sceneGraph) {
	this->init();
}

UiElement::~UiElement() {
	this->destroy();
}

void UiElement::InitSprite(unsigned int width, unsigned int height, std::string shaderName_, std::vector<std::string> pathsToTextures, bool hasTransperancy) {
	this->addChildSpriteObject();

	if (width >= this->GetWidth()) {
		this->setWidth(width);
	}
	if (height >= this->GetHeight()) {
		this->setHeight(height);
	}
	//
	this->childSpriteObjectRef->InitSprite(width, height, shaderName_, pathsToTextures, hasTransperancy);
}

void UiElement::InitSprite(unsigned int width, unsigned int height, std::string shaderName_, glm::vec4 color) {
	this->addChildSpriteObject();

	if (width >= this->GetWidth()) {
		this->setWidth(width);
	}
	if (height >= this->GetHeight()) {
		this->setHeight(height);
	}
	//
	this->childSpriteObjectRef->InitSprite(width, height, shaderName_, color);
}

void UiElement::InitTextToDisplay(std::string text, unsigned int width, unsigned int height, std::string pathToFontSpecificationFile, float fontSize) {
	if (this->childFontObjectRef == nullptr) {
		this->addChildFontObject();
	}

	if (width >= this->GetWidth()) {
		this->setWidth(width);
	}
	if (height >= this->GetHeight()) {
		this->setHeight(height);
	}

	this->childFontObjectRef->InitTextToDisplay(text, width, height, pathToFontSpecificationFile);

	this->childFontObjectRef->GetTransform()->SetScale(fontSize, fontSize, fontSize);
}

unsigned int UiElement::GetSpriteTextureIndex() {
	SpriteRenderer* spriteRenderer = this->childSpriteObjectRef->GetSpriteRenderer();
	if (spriteRenderer != nullptr) {
		return spriteRenderer->GetCurrentTextureIndex();
	}
	return 0xFFFFFFFF;
}

void UiElement::SetSpriteTextureIndex(unsigned int textureIndex) {
	SpriteRenderer* spriteRenderer = this->childSpriteObjectRef->GetSpriteRenderer();
	if (spriteRenderer != nullptr) {
		spriteRenderer->SetCurrentTextureIndex(textureIndex);
	}
}

void UiElement::SetFontColor(glm::vec4 color) {
	if (this->childFontObjectRef != nullptr) {
		this->childFontObjectRef->SetFontColor(color);
	}
}

void UiElement::init() {
	this->setClassType(CLASS_TYPE_UIELEMENT);

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

std::string UiElement::GetPathToFontFile() {
	if (this->childFontObjectRef != nullptr) {
		return this->childFontObjectRef->getPathToFontFile();
	}
	return "";
}

