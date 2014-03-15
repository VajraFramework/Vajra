//
//  GridZoneSwitch.cpp
//  Created by Matt Kaufmann on 02/12/14.
//

#include "ExampleGame/Components/Switches/GridZoneSwitch.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Common/Objects/Object.h"

GridZoneSwitch::GridZoneSwitch() : BaseSwitch() {
	this->init();
}

GridZoneSwitch::GridZoneSwitch(Object* object_) : BaseSwitch(object_) {
	this->init();
}

GridZoneSwitch::~GridZoneSwitch() {
	this->destroy();
}

void GridZoneSwitch::init() {
	this->requiredOccupants = 1;
	this->activateWhenUnitsInZone = true;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_ENTERED, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_EXITED, this->GetTypeId(), true);
}

void GridZoneSwitch::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());

	this->occupants.clear();
}

void GridZoneSwitch::SetSwitchType(std::string typeStr) {
	BaseSwitch::SetSwitchType(typeStr);
}

void GridZoneSwitch::SetResetTime(float t) {
	BaseSwitch::SetResetTime(t);
}

void GridZoneSwitch::SetRequiredOccupants(unsigned int num) {
	this->requiredOccupants = num;
}

void GridZoneSwitch::SetActivateWhenUnitsInZone(bool b) {
	this->activateWhenUnitsInZone = b;
}

void GridZoneSwitch::HandleMessage(MessageChunk messageChunk) {
	BaseSwitch::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_GRID_ZONE_ENTERED:
			this->onUnitEnteredZone(messageChunk->GetSenderId());
			break;

		case MESSAGE_TYPE_GRID_ZONE_EXITED:
			this->onUnitExitedZone(messageChunk->messageData.iv1.x);
			break;
	}
}

bool GridZoneSwitch::isConditionMet() {
	return this->occupants.size() >= this->requiredOccupants;
}

bool GridZoneSwitch::doesObjectCountAsOccupant(ObjectIdType /*id*/) {
	return true;
}

void GridZoneSwitch::onUnitEnteredZone(ObjectIdType id) {
	auto it = std::find(this->occupants.begin(), this->occupants.end(), id);
	if (it == this->occupants.end()) {
		if (this->doesObjectCountAsOccupant(id)) {
			this->occupants.push_back(id);

			if (this->occupants.size() >= this->requiredOccupants) {
				this->setConditionState(this->activateWhenUnitsInZone);
			}
		}
	}
	else {
		FRAMEWORK->GetLogger()->dbglog("\nObject %d is already in zone: %d", id, this->GetObject()->GetId());
	}
}

void GridZoneSwitch::onUnitExitedZone(ObjectIdType id) {
	auto it = std::find(this->occupants.begin(), this->occupants.end(), id);
	if (it != this->occupants.end()) {
		this->occupants.erase(it);

		if (this->occupants.size() < this->requiredOccupants) {
			this->setConditionState(!this->activateWhenUnitsInZone);
		}
	}
	else {
		FRAMEWORK->GetLogger()->dbglog("\nObject %d is not in grid zone: %d", id, this->GetObject()->GetId());
	}
}

void GridZoneSwitch::SetDecalType(std::string decalType) {
	this->setDecalType(decalType);
}
