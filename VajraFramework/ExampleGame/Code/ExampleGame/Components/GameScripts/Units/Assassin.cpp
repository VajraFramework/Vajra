#include "ExampleGame/Components/GameScripts/Units/Assassin.h"
#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"

#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"

#define ATTACK_SPEED 10.0f
#define SWIPE_DIST_IN_PIXELS 50.0f
#define SWIPE_DURATION_IN_SECONDS 1.0f
#define DASH_DISTANCE_IN_UNITS 6

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
		this->trySpecial(touchId);
	}
}

void Assassin::trySpecial(int /*touchId*/) {
	if(this->targetedCell != nullptr && this->targetedCell != this->gridNavRef->GetCurrentCell()) {
		this->startSpecial();
	} else {
		this->onSpecialEnd();
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

void Assassin::touchedCellChanged(GridCell* prevTouchedCell) {
	if(this->inputState == InputState::INPUT_STATE_NAV) {
		PlayerUnit::touchedCellChanged(prevTouchedCell);
	} else {
		std::list<GridCell*> touchedCells;
		SINGLETONS->GetGridManager()->GetGrid()->TouchedCells(this->gridNavRef->GetCurrentCell(), this->GetCurrentTouchedCell(), touchedCells);
		int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(this->gridNavRef->GetCurrentCell()->center.y);
		int cellIndex = 0;
		for( GridCell* c : touchedCells) {
			if(cellIndex <= DASH_DISTANCE_IN_UNITS && SINGLETONS->GetGridManager()->GetGrid()->IsCellPassableAtElevation(c->x, c->z, elevation)) {
				this->targetedCell = c;
			} else {
				break;
			}
			cellIndex++;
		}
		this->GetTouchIndicator()->GetTransform()->SetPosition(this->targetedCell->center);
		this->GetTouchIndicator()->GetTransform()->Translate(0.01f, YAXIS);
	}
	
}

