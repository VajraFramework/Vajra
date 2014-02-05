#include "ExampleGame/Components/GameScripts/Units/Thief.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"

#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/Tween/Tween.h"


static Thief* g_thief;
namespace ThiefTween {
	void tweenCallback(ObjectIdType /* gameObjectId */, std::string /* tweenClipName */) {
		g_thief->onSpecialEnd();
	}
}
Thief::Thief() : PlayerUnit() {
	this->init();
}

Thief::Thief(Object* object_) : PlayerUnit(object_) {
	this->init();
}

Thief::~Thief() {
	this->destroy();
}

void Thief::init() {
	g_thief = this;
	this->unitType = UnitType::UNIT_TYPE_THIEF;
}

void Thief::destroy() {
}

bool Thief::isSpecialTouch(int touchId) {
	if(this->getTouchNearUnit()) {
		Touch touch = ENGINE->GetInput()->GetTouch(touchId);

	
		//if(touch.timeDown >= longPressInSeconds && glm::distance(touch.pos, this->touchStartPos) <= allowedMovementInPixels) {
			this->targetedCell = nullptr;
			return true;
		//}
	}
	return false;
}

void Thief::onSpecialTouch(int touchId) {
	Touch touch = ENGINE->GetInput()->GetTouch(touchId);
	if(touch.phase == TouchPhase::Ended) {
		this->targetedCell = SINGLETONS->GetGridManager()->TouchPositionToCell(touch.pos);
		this->startSpecial();
	}
}

void Thief::startSpecial() {
	PlayerUnit::startSpecial();
	ENGINE->GetTween()->TweenPosition(this->gameObjectRef->GetId(),
											this->gameObjectRef->GetTransform()->GetPosition(),
											this->targetedCell->center,
											1.0f,
											false,
											TWEEN_TRANSLATION_CURVE_TYPE_PARABOLA, 
											false,
											ThiefTween::tweenCallback);
}

void Thief::onSpecialEnd() {
	PlayerUnit::onSpecialEnd();
	this->gridNavRef->SetGridPosition(this->targetedCell->center.x, this->targetedCell->center.z);

}