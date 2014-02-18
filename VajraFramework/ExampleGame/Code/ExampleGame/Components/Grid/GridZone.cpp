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
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"

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
	Transform* trans = this->GetObject()->GetComponent<Transform>();

	int centerX, centerZ;
	SINGLETONS->GetGridManager()->GetGrid()->GetCoordinates(centerX, centerZ, trans->GetPositionWorld());

	glm::vec3 forward = trans->GetForward();

	// Determine the rough orientation of the object.
	if (abs(forward.z) >= abs(forward.x)) {
		if (forward.z >= 0.0f) {
			west  = centerX + this->westBound;
			east  = centerX + this->eastBound;
			south = centerZ + this->southBound;
			north = centerZ + this->northBound;
		}
		else {
			west  = centerX - this->eastBound;
			east  = centerX - this->westBound;
			south = centerZ - this->northBound;
			north = centerZ - this->southBound;
		}
	}
	else {
		if (forward.x >= 0.0f) {
			west  = centerX - this->northBound;
			east  = centerX - this->southBound;
			south = centerZ + this->westBound;
			north = centerZ + this->eastBound;
		}
		else {
			west  = centerX + this->southBound;
			east  = centerX + this->northBound;
			south = centerZ - this->eastBound;
			north = centerZ - this->westBound;
		}
	}
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
		int west, east, south, north;

		this->GetZoneBounds(west, east, south, north);

		return ((cell->x >= west) && (cell->x <= east) && (cell->z >= south) && (cell->z <= north));
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
