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

void GridZone::GetZoneBounds(int& west, int& east, int& south, int& north) {
	west  = this->westBound;
	east  = this->eastBound;
	south = this->southBound;
	north = this->northBound;
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

bool GridZone::IsCellWithinZone(GridCell* cell) {
	if (cell != nullptr) {
		return ((cell->x >= this->westBound) && (cell->x <= this->eastBound) && (cell->z >= this->southBound) && (cell->z <= this->northBound));
	}

	return false;
}

void GridZone::init() {
	this->zoneType = GRID_ZONE_TYPE_UNKNOWN;
	this->westBound = -1;
	this->eastBound = -1;
	this->southBound = -1;
	this->northBound = -1;

	SINGLETONS->GetGridManager()->GetGrid()->AddGridZone(this->GetObject()->GetId());
}

void GridZone::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
