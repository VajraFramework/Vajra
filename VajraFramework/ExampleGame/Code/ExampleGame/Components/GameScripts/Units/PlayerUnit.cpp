#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"


PlayerUnit::PlayerUnit() : BaseUnit() {
	this->init();
}

PlayerUnit::PlayerUnit(Object* object_) : BaseUnit(object_) {
	this->init();
}

PlayerUnit::~PlayerUnit() {
	this->destroy();
}

void PlayerUnit::init() {
	this->unitType = UnitType::UNIT_TYPE_ASSASSIN;
}

void PlayerUnit::destroy() {
}

void PlayerUnit::OnTouch(int touchId, GridCell* touchedCell) {
	switch(this->inputState) {
		case InputState::INPUT_STATE_WAIT:
			this->onSelectedTouch();
			break;
		case InputState::INPUT_STATE_NAV:
			this->onNavTouch(touchId, touchedCell);
			break;
		case InputState::INPUT_STATE_SPECIAL:
			this->onSpecialTouch(touchId);
			break;
		default:
			break;
	}
}

void PlayerUnit::OnDeselect() {
	this->inputState = InputState::INPUT_STATE_WAIT;
}
void PlayerUnit::onSelectedTouch() {
	this->inputState = InputState::INPUT_STATE_NAV;
}

void PlayerUnit::onNavTouch(int touchId, GridCell* touchedCell) {
	if(this->isSpecialTouch(touchId)) {

	}
	else {
		this->gridNavRef->SetDestination(touchedCell->x, touchedCell->z);
	}

}