//
//  GridCell.cpp
//  Created by Matt Kaufmann on 12/10/13.
//

#include "ExampleGame/Components/Grid/GridCell.h"

GridCell::GridCell(int x0, int y0, int z0, glm::vec3 origin0, glm::vec3 center0) :
	x(x0),
	y(y0),
	z(z0),
	origin(origin0),
	center(center0)
{
	for (int i = 0; i < NUM_ELEVATIONS; ++i) {
		this->unitIds[i]    = OBJECT_ID_INVALID;
	}
}

GridCell::~GridCell() {
	this->staticObjs.clear();
}

ObjectIdType GridCell::GetFirstOccupantId() {
	for (int i = 0; i < NUM_ELEVATIONS; ++i) {
		if (this->unitIds[i] != OBJECT_ID_INVALID) {
			return this->unitIds[i];
		}
	}
	return OBJECT_ID_INVALID;
}

ObjectIdType GridCell::GetOccupantIdAtElevation(int elevation) {
	int index = elevation - this->y;
	if ((index >= 0) && (index < NUM_ELEVATIONS)) {
		return this->unitIds[index];
	}
	return OBJECT_ID_INVALID;
}

ObjectIdType GridCell::GetTopOccupantId() {
	for (int i = NUM_ELEVATIONS - 1; i >= 0; --i) {
		if (this->unitIds[i] != OBJECT_ID_INVALID) {
			return this->unitIds[i];
		}
	}
	return OBJECT_ID_INVALID;
}

void GridCell::SetFirstOccupantId(ObjectIdType id) {
	this->SetOccupantIdAtElevation(id, this->y);
}

void GridCell::SetOccupantIdAtElevation(ObjectIdType id, int elevation) {
	// The occupant at the cell's ground level will always be at index 0.
	int index = elevation - this->y;
	if ((index >= 0) && (index < NUM_ELEVATIONS)) {
		// Set the occupant ID at that index
		this->unitIds[index] = id;
	}
}

void GridCell::AddZoneToCell(ObjectIdType id) {
	auto iter = std::find(this->zones.begin(), this->zones.end(), id);
	if (iter == this->zones.end()) {
		this->zones.push_back(id);
	}
}

void GridCell::RemoveZoneFromCell(ObjectIdType id) {
	auto iter = std::find(this->zones.begin(), this->zones.end(), id);
	if (iter != this->zones.end()) {
		this->zones.erase(iter);
	}
}

ObjectIdType GridCell::GetFirstZoneId() {
	if (this->zones.empty()) {
		return OBJECT_ID_INVALID;
	}
	else {
		return this->zones.front();
	}
}

void GridCell::GetZoneIds(std::list<ObjectIdType>& ids) {
	for (auto iter = this->zones.begin(); iter != this->zones.end(); ++iter) {
		ids.push_back(*iter);
	}
}
