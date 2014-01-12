#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
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

void UiObject::SetClickable(bool clickable_) {
	if (!this->clickable && clickable_) {
		ENGINE->GetSceneGraphUi()->GetUiTouchManager()->RegisterTouchableUiElement(this->GetId());
	} else if (this->clickable && !clickable_) {
		ENGINE->GetSceneGraphUi()->GetUiTouchManager()->UnRegisterTouchableUiElement(this->GetId());
	}
	this->clickable = clickable_;
}

void UiObject::OnClick(Touch touch) {
	FRAMEWORK->GetLogger()->dbglog("\nid %d, Got touched: %f, %f", this->GetId(), touch.pos.x, touch.pos.y);
}

void UiObject::OnTouchMove(Touch touch) {
	FRAMEWORK->GetLogger()->dbglog("\nid %d, Mouse move: %f, %f", this->GetId(), touch.pos.x, touch.pos.y);
}

void UiObject::init() {
	this->width  = 0;
	this->height = 0;
	//
	this->clickable = false;
}

void UiObject::destroy() {
}
