#include "Vajra/Engine/Components/DerivedComponents/Renderer/UiSpriteRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Ui/UiSpriteObject/UiSpriteObject.h"

UiSpriteObject::UiSpriteObject(SceneGraph* sceneGraph) : GameObject(sceneGraph) {
	this->init();
}

UiSpriteObject::~UiSpriteObject() {
	this->destroy();
}

void UiSpriteObject::InitSprite(unsigned int width, unsigned int height, std::string shaderName_, std::string pathToTexture /* = "" */) {
	ASSERT(this->GetComponent<UiSpriteRenderer>() == nullptr, "UiObject doesn't already have a UiRenderer on it");
	UiSpriteRenderer* uiRenderer = this->AddComponent<UiSpriteRenderer>();
	uiRenderer->initPlane(width, height, shaderName_, pathToTexture);
	//
	this->setWidth(width);
	this->setHeight(height);
}

void UiSpriteObject::InitSprite(unsigned int width, unsigned int height, std::string shaderName_, glm::vec4 color) {
	ASSERT(this->GetComponent<UiSpriteRenderer>() == nullptr, "UiObject doesn't already have a UiRenderer on it");
	UiSpriteRenderer* uiRenderer = this->AddComponent<UiSpriteRenderer>();
	uiRenderer->initPlane(width, height, shaderName_);
	uiRenderer->setDiffuseColor(color);
	//
	this->setWidth(width);
	this->setHeight(height);
}

void UiSpriteObject::SetPosition(unsigned int x, unsigned int y) {
	// TODO [Implement] Implement zorder
	this->GetTransform()->SetPosition(x, -1.0f * y, 0.0f);
}

void UiSpriteObject::init() {
	this->width  = 0;
	this->height = 0;
	//
	this->clickable = false;
}

void UiSpriteObject::destroy() {
}
