#include "ExampleGame/Components/GameScripts/Units/Assassin.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"

#define ATTACK_SPEED 10.0f
#define SWIPE_DIST_IN_PIXELS 50.0f
#define SWIPE_DURATION_IN_SECONDS 1.0f

Assassin::Assassin() : PlayerUnit() {
	this->init();
}

Assassin::Assassin(Object* object_) : PlayerUnit(object_) {
	this->init();
}

Assassin::~Assassin() {
	this->destroy();
}

void Assassin::init() {
	this->unitType = UnitType::UNIT_TYPE_ASSASSIN;
}

void Assassin::destroy() {
}

bool Assassin::isSpecialTouch(int touchId) {
	if(this->getTouchNearUnit()) {
		Touch touch = ENGINE->GetInput()->GetTouch(touchId);
		if(touch.timeDown <= SWIPE_DURATION_IN_SECONDS && glm::distance(touch.pos, this->touchStartPos) > SWIPE_DIST_IN_PIXELS) {
			this->swipeDirectionScreen = this->touchStartPos - touch.pos;
			this->targetedCell = nullptr;
			return true;
		}
	}
	return false;
}

void Assassin::onSpecialTouch(int touchId) {
	Touch touch = ENGINE->GetInput()->GetTouch(touchId);
	if(touch.phase == TouchPhase::Ended) {
		this->targetedCell = SINGLETONS->GetGridManager()->TouchPositionToCell(touch.pos);
		this->startSpecial();
	}
}

void Assassin::startSpecial() {
	PlayerUnit::startSpecial();
	this->gridNavRef->SetMovementSpeed(ATTACK_SPEED);
	this->gridNavRef->SetDestination(this->targetedCell->x, this->targetedCell->z);
}

void Assassin::onSpecialEnd() {
	PlayerUnit::onSpecialEnd();
	this->gridNavRef->SetMovementSpeed(MOVE_SPEED);

}
