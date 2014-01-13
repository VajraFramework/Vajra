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
	Touch t = ENGINE->GetInput()->GetTouch(touchId);
	switch(this->inputState) {
		case InputState::INPUT_STATE_WAIT:
			this->OnSelectedTouch();
			break;
		case InputState::INPUT_STATE_NAV:
			this->OnNavTouch(touchId, touchedCell);
			break;
		case InputState::INPUT_STATE_SPECIAL:
			this->OnSpecialTouch(touchId);
			break;
		default:
			break;
	}
}

void PlayerUnit::OnDeselect() {
	this->inputState = InputState::INPUT_STATE_WAIT;
}
void PlayerUnit::OnSelectedTouch() {
	this->inputState = InputState::INPUT_STATE_NAV;
}

void PlayerUnit::OnNavTouch(int touchId, GridCell* touchedCell) {
	this->gridNavRef->SetDestination(touchedCell->x, touchedCell->z);

}