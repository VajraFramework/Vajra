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

GridZone::GridZone() : Component() {
	this->init();
}

GridZone::GridZone(Object* object_) : Component(object_) {
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

bool GridZone::CellWithinZone(GridCell* cell) {
	if (cell == nullptr) {
		return false;
	}

	return ((cell->x >= this->westBound) && (cell->x <= this->eastBound) && (cell->z >= this->southBound) && (cell->z <= this->northBound));
}

unsigned int GridZone::CollisionCheck(GridCell* startCell, GridCell* destCell) {
	bool startInZone = CellWithinZone(startCell);
	bool endInZone = CellWithinZone(destCell);

	if (!startInZone && endInZone) {
		return MESSAGE_TYPE_GRID_ZONE_ENTERED;
	}
	if (startInZone && !endInZone) {
		return MESSAGE_TYPE_GRID_ZONE_EXITED;
	}
	return MESSAGE_TYPE_UNSPECIFIED;
}

void GridZone::init() {
	this->westBound = -1;
	this->eastBound = -1;
	this->southBound = -1;
	this->northBound = -1;

	SINGLETONS->GetGridManager()->AddGridZone(this->GetObject()->GetId());
}

void GridZone::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
