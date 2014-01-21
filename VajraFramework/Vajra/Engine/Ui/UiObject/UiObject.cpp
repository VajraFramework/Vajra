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

void UiObject::SetClickable(bool clickable_, UiTouchHandlers* touchHandlers_ /* = nullptr */) {
	if (!this->clickable && clickable_) {
		VERIFY(touchHandlers_ != nullptr, "Valid touch handler provided");
		ENGINE->GetSceneGraphUi()->GetUiTouchManager()->RegisterTouchableUiElement(this->GetId());
		this->touchHandlers = touchHandlers_;
	} else if (this->clickable && !clickable_) {
		ENGINE->GetSceneGraphUi()->GetUiTouchManager()->UnRegisterTouchableUiElement(this->GetId());
		this->touchHandlers = nullptr;
	}
	this->clickable = clickable_;
}

void UiObject::OnTouchDown(Touch touch) {
	ASSERT(this->touchHandlers != nullptr, "Valid touch handler present");
	this->touchHandlers->OnTouchDownHandlers(this, touch);
}

void UiObject::OnTouchMove(Touch touch) {
	ASSERT(this->touchHandlers != nullptr, "Valid touch handler present");
	this->touchHandlers->OnTouchMoveHandlers(this, touch);
}

void UiObject::init() {
	this->width  = 0;
	this->height = 0;
	//
	this->clickable = false;
	this->touchHandlers = nullptr;
}

void UiObject::destroy() {
	if (this->clickable) {
		ENGINE->GetSceneGraphUi()->GetUiTouchManager()->UnRegisterTouchableUiElement(this->GetId());
	}
}
