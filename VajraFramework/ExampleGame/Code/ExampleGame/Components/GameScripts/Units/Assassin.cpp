#include "ExampleGame/Components/GameScripts/Units/Assassin.h"
#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameConstants/GameConstants.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"

#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"

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
		if(touch.timeDown <= GetFloatGameConstant(GAME_CONSTANT_swipe_duration_in_seconds) && glm::distance(touch.pos, this->touchStartPos) > GetFloatGameConstant(GAME_CONSTANT_swipe_dist_in_pixels)) {
			this->swipeDirectionScreen = this->touchStartPos - touch.pos;
			this->targetedCell = nullptr;
			this->SetTouchIndicatorSprite(ASSASSIN_SPECIAL);
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
		this->onSpecialCancelled();
	}
}

void Assassin::startSpecial() {
	PlayerUnit::startSpecial();
	this->gridNavRef->SetMovementSpeed(GetFloatGameConstant(GAME_CONSTANT_assassin_attack_speed));
	this->gridNavRef->SetDestination(this->targetedCell->x, this->targetedCell->z);
	this->startTouchIndicatorPulse();
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
			if(cellIndex <= GetFloatGameConstant(GAME_CONSTANT_dash_distance_in_units) && SINGLETONS->GetGridManager()->GetGrid()->IsCellPassableAtElevation(c->x, c->z, elevation)) {
				this->targetedCell = c;
			} else {
				break;
			}
			cellIndex++;
		}
		this->SetTouchIndicatorCell(this->targetedCell);
	}
}
