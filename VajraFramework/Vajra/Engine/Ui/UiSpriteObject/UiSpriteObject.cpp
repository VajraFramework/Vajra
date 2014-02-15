#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Ui/UiSpriteObject/UiSpriteObject.h"

UiSpriteObject::UiSpriteObject(SceneGraph* sceneGraph) : UiObject(sceneGraph) {
	this->init();
}

UiSpriteObject::~UiSpriteObject() {
	this->destroy();
}

void UiSpriteObject::InitSprite(unsigned int width, unsigned int height, std::string shaderName_, std::vector<std::string> pathsToTextures, bool hasTransperancy) {
	ASSERT(this->GetComponent<Renderer>() == nullptr, "UiObject doesn't already have a Renderer on it");
	SpriteRenderer* uiRenderer = this->AddComponent<SpriteRenderer>();
	uiRenderer->initPlane(width, height, shaderName_, pathsToTextures);
	uiRenderer->SetHasTransperancy(hasTransperancy);
	//
	this->setWidth(width);
	this->setHeight(height);
}

void UiSpriteObject::InitSprite(unsigned int width, unsigned int height, std::string shaderName_, glm::vec4 color) {
	ASSERT(this->GetComponent<Renderer>() == nullptr, "UiObject doesn't already have a Renderer on it");
	std::vector<std::string> empthPathsToTextures;
	SpriteRenderer* uiRenderer = this->AddComponent<SpriteRenderer>();
	uiRenderer->initPlane(width, height, shaderName_, empthPathsToTextures);
	uiRenderer->setDiffuseColor(color);
	// TODO [Hack] Treating alphas greater than 0.98 as opaque to avoid floating point issues
	if (color.a < 0.98f) {
		uiRenderer->SetHasTransperancy(true);
	}
	//
	this->setWidth(width);
	this->setHeight(height);
}

void UiSpriteObject::init() {
}

void UiSpriteObject::destroy() {
}
