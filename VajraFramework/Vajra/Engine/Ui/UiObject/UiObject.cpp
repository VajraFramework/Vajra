#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"

UiObject::UiObject(SceneGraph* sceneGraph) : GameObject(sceneGraph) {
	this->init();
}

UiObject::~UiObject() {
	this->destroy();
}

void UiObject::SetPosition(unsigned int x, unsigned int y) {
	// TODO [Implement] Implement zorder
	this->GetTransform()->SetPosition(x, -1.0f * y, 0.0f);
}

void UiObject::init() {
	this->width  = 0;
	this->height = 0;
	//
	this->clickable = false;
}

void UiObject::destroy() {
}
