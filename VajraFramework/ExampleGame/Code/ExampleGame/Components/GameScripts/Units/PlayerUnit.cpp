#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
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
	this->inputState = InputState::INPUT_STATE_WAIT;
	this->touchNearUnit = false;
	this->performingSpecial = false;

	this->moveSpeed = 2.5f;
	this->turnSpeedDegrees = 360.0f;
	this->gridNavRef->SetMovementSpeed(this->moveSpeed);
	this->gridNavRef->SetTurnSpeedDegrees(this->turnSpeedDegrees);

	this->addSubscriptionToMessageType(MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION, this->GetTypeId(), false);
}

void PlayerUnit::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void PlayerUnit::HandleMessage(MessageChunk messageChunk) {
	BaseUnit::HandleMessage(messageChunk);
	switch(messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION:
			if(this->performingSpecial) {
				onSpecialEnd();
			}
			break;
		default:
			break;
	}
}

void PlayerUnit::OnTouch(int touchId, GridCell* touchedCell) {

	if(ENGINE->GetInput()->GetTouch(touchId).phase == TouchPhase::Began) {
		this->touchStartPos = ENGINE->GetInput()->GetTouch(touchId).pos;
		this->setTouchNearUnit();
	}	

	if(this->performingSpecial) {
		return;
	}

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

void PlayerUnit::startSpecial() {
	this->performingSpecial = true;
}

void PlayerUnit::onSpecialEnd() {
	this->performingSpecial = false;
	this->inputState = InputState::INPUT_STATE_NAV;
}
void PlayerUnit::onNavTouch(int touchId, GridCell* touchedCell) {
	if(this->isSpecialTouch(touchId)) {
		this->inputState = InputState::INPUT_STATE_SPECIAL;
		this->gridNavRef->StopNavigation();

	}
	else {
		if(ENGINE->GetInput()->GetTouch(touchId).phase == TouchPhase::Ended) {
			this->gridNavRef->SetDestination(touchedCell->x, touchedCell->z);
		}
	}

}

void PlayerUnit::setTouchNearUnit() {
	glm::vec3 gridPos = SINGLETONS->GetGridManager()->TouchPositionToGridPosition(touchStartPos);
	if(glm::distance(gridPos, this->gameObjectRef->GetTransform()->GetPosition()) < 1.5f) {
		this->touchNearUnit = true;
	} else {
		this->touchNearUnit = false;
	}
}
