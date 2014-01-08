//
//  GridZone.cpp
//  Created by Matt Kaufmann on 01/07/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridZone.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"

unsigned int GridZone::componentTypeId = COMPONENT_TYPE_ID_GRID_ZONE;

GridZone::GridZone() : GameScript() {
	this->init();
}

GridZone::GridZone(Object* object_) : GameScript(object_) {
	this->init();
}

GridZone::~GridZone() {
	this->destroy();
}

void GridZone::SetZoneBounds(int xMin, int zMin, int xMax, int zMax) {
	if (xMin <= xMax) {
		this->westBound = xMin;
		this->eastBound = xMax;
	}
	else {
		this->westBound = xMax;
		this->eastBound = xMin;
	}
	if (zMin <= zMax) {
		this->southBound = zMin;
		this->northBound = zMax;
	}
	else {
		this->southBound = zMax;
		this->northBound = zMin;
	}
}

unsigned int GridZone::CollisionCheck(GridCell* startCell, GridCell* destCell) {
	bool startInZone = false;
	if (startCell != nullptr) {
		if ((startCell->x >= this->westBound) && (startCell->x <= this->eastBound) && (startCell->z >= this->southBound) && (startCell->z <= this->northBound)) {
			startInZone = true;
		}
	}

	bool endInZone = false;
	if (destCell != nullptr) {
		if ((destCell->x >= this->westBound) && (destCell->x <= this->eastBound) && (destCell->z >= this->southBound) && (destCell->z <= this->northBound)) {
			endInZone = true;
		}
	}

	if (!startInZone && endInZone) {
		return MESSAGE_TYPE_GRID_ZONE_ENTERED;
	}
	if (startInZone && !endInZone) {
		return MESSAGE_TYPE_GRID_ZONE_EXITED;
	}
	return MESSAGE_TYPE_UNSPECIFIED;
}

void GridZone::HandleMessage(MessageChunk messageChunk) {
	switch(messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_GRID_ZONE_ENTERED:
			break;

		case MESSAGE_TYPE_GRID_ZONE_EXITED:
			break;
	default:
		break;
	}
}

void GridZone::init() {
	this->westBound = -1;
	this->eastBound = -1;
	this->southBound = -1;
	this->northBound = -1;

	SINGLETONS->GetGridManager()->AddGridZone(this->GetObject()->GetId());

	// Subscribe to "collision" messages
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_ENTERED, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_EXITED, this->GetTypeId(), false);
}

void GridZone::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
