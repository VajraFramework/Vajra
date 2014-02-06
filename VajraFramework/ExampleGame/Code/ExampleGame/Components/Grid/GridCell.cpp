//
//  GridCell.cpp
//  Created by Matt Kaufmann on 12/10/13.
//

#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridConstants.h"

GridCell::GridCell(int x0, int y0, int z0, glm::vec3 origin0, glm::vec3 center0) :
	x(x0),
	y(y0),
	z(z0),
	origin(origin0),
	center(center0)
{ }

ObjectIdType GridCell::GetFirstOccupantId() {
	for (auto iter = this->unitIds.begin(); iter != this->unitIds.end(); ++iter) {
		if (*iter != OBJECT_ID_INVALID) {
			return *iter;
		}
	}
	return OBJECT_ID_INVALID;
}

ObjectIdType GridCell::GetOccupantIdAtElevation(int elevation) {
	int index = elevation - this->y;

	if ((index >= 0) && (index < (int)this->unitIds.size())) {
		int i = 0;
		auto iter = this->unitIds.begin();
		while (i < index) {
			++iter;
		}
		return *iter;
	}
	return OBJECT_ID_INVALID;
}

ObjectIdType GridCell::GetTopOccupantId() {
	for (auto iter = this->unitIds.rbegin(); iter != this->unitIds.rend(); ++iter) {
		if (*iter != OBJECT_ID_INVALID) {
			return *iter;
		}
	}
	return OBJECT_ID_INVALID;
}

void GridCell::SetFirstOccupantId(ObjectIdType id) {
	this->SetOccupantIdAtElevation(id, this->y);
}

void GridCell::SetOccupantIdAtElevation(ObjectIdType id, int elevation) {
	int index = elevation - this->y;

	if ((index >= 0) && (index < (int)this->unitIds.size())) {
		int i = 0;
		auto iter = this->unitIds.begin();
		while (i < index) {
			++iter;
		}
		*iter = id;

		if (id == OBJECT_ID_INVALID) {
			int count = this->unitIds.size();
			for (auto iter = this->unitIds.rbegin(); iter != this->unitIds.rend(); ++iter) {
				if (*iter == OBJECT_ID_INVALID) {
					--count;
				}
				else {
					break;
				}
			}

			while (count > 0) {
				this->unitIds.pop_back();
				--count;
			}
		}
	}
}
