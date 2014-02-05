#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Messages/Declarations.h"

#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"

ComponentIdType BaseUnit::componentTypeId = COMPONENT_TYPE_ID_BASE_UNIT;

BaseUnit::BaseUnit() : Component() {
	this->init();
}

BaseUnit::BaseUnit(Object* object_) : Component(object_) {
	this->init();
}

BaseUnit::~BaseUnit() {
	this->destroy();
}

void BaseUnit::init() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SCENE_START, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SCENE_END  , this->GetTypeId(), false);

	this->gameObjectRef = (GameObject*)this->GetObject();
	this->gridNavRef = this->gameObjectRef->GetComponent<GridNavigator>();
	this->unitState = UnitState::UNIT_STATE_ALIVE;
	this->unitType = UnitType::UNIT_TYPE_GUARD;
	this->unitActionState = UnitActionState::UNIT_ACTION_STATE_IDLE;
}

void BaseUnit::destroy() {
	this->end();
}

void BaseUnit::start() {
}

void BaseUnit::end() {
	this->gridNavRef = nullptr;
}

void BaseUnit::update() {
}

void BaseUnit::Kill() {
	this->unitState = UnitState::UNIT_STATE_DEAD;

	// broad cast a message about unit death
	GridCell* currentCell = this->gridNavRef->GetCurrentCell();
	MessageChunk unitKilledMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	unitKilledMessage->SetMessageType(MESSAGE_TYPE_UNIT_KILLED);
	unitKilledMessage->messageData.i = this->unitType;
	unitKilledMessage->messageData.fv1.x = currentCell->x;
	unitKilledMessage->messageData.fv1.y = currentCell->y;
	unitKilledMessage->messageData.fv1.z = currentCell->z;
	ENGINE->GetMessageHub()->SendMulticastMessage(unitKilledMessage, this->GetObject()->GetId());

	// Remove the navigator component
	this->gameObjectRef->RemoveComponent<GridNavigator>();
	this->gridNavRef = nullptr;
}
