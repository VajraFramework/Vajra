#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Engine/Ui/UiTouchHandlers/UiTouchHandlers.h"
#include "Vajra/Engine/Ui/UiTouchManager/UiTouchManager.h"

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

void UiObject::SetZOrder(int zorder_) {
	glm::vec3 position = this->GetTransform()->GetPosition();
	this->GetTransform()->SetPosition(position.x, position.y, zorder_);
	this->zorder = zorder_;
}

bool UiObject::IsPointWithin(float x, float y) {
	glm::vec3 position = this->GetTransform()->GetPositionWorld();
	glm::vec3 scale    = this->GetTransform()->GetScaleWorld();
	//
	position.y *= -1.0f;
	//
	if (x >= position.x && x <= (position.x + this->width  * scale.x) &&
		y >= position.y && y <= (position.y + this->height * scale.y) ) {
		return true;
	}
	return false;
}

void UiObject::SetVisible(bool visible_) {
	if(visible_ && !this->IsVisible() && this->clickable) {
		this->registerWithTouchHandlers();
	}
	else if(!visible_ && this->IsVisible() && this->clickable ) {
		this->unregisterWithTouchHandlers();
	}
	GameObject::SetVisible(visible_);
}

void UiObject::SetClickable(bool clickable_) {
	if(clickable_ && !this->clickable && this->IsVisible()) {
		this->registerWithTouchHandlers();
	}
	else if(!clickable_ && this->clickable && this->IsVisible()) {
		this->unregisterWithTouchHandlers();
	}
	this->clickable = clickable_;
}

void UiObject::SetTouchHandlers(UiTouchHandlers* touchHandlers_ /* = nullptr */) {
	VERIFY(touchHandlers_ != nullptr, "Valid touch handler provided");
	this->touchHandlers = touchHandlers_;
} 

void UiObject::OnTouchDown(Touch touch) {
	ASSERT(this->touchHandlers != nullptr, "Valid touch handler present");
	this->touchHandlers->OnTouchDownHandlers(this, touch);
}

void UiObject::OnTouchMove(Touch touch) {
	ASSERT(this->touchHandlers != nullptr, "Valid touch handler present");
	this->touchHandlers->OnTouchMoveHandlers(this, touch);
}

void UiObject::OnTouchUp(Touch touch) {
	ASSERT(this->touchHandlers != nullptr, "Valid touch handler present");
	this->touchHandlers->OnTouchUpHandlers(this, touch);
}

void UiObject::init() {
	this->setClassType(CLASS_TYPE_UIOBJECT);

	this->width  = 0;
	this->height = 0;
	//
	this->clickable = false;
	this->touchHandlers = nullptr;
}

void UiObject::destroy() {
	if (this->clickable && this->IsVisible()) {
		ENGINE->GetSceneGraphUi()->GetUiTouchManager()->UnRegisterTouchableUiElement(this->GetId());
	}
}

void UiObject::registerWithTouchHandlers() {
	VERIFY(this->touchHandlers != nullptr, "Valid touch handler exits");
	ENGINE->GetSceneGraphUi()->GetUiTouchManager()->RegisterTouchableUiElement(this->GetId());
}

void UiObject::unregisterWithTouchHandlers() {
	ENGINE->GetSceneGraphUi()->GetUiTouchManager()->UnRegisterTouchableUiElement(this->GetId());
}
