//
//  GridCell.h
//  Created by Matt Kaufmann on 12/10/13.
//

#ifndef GRIDCELL_H
#define GRIDCELL_H

#include "Vajra/Engine/GameObject/GameObject.h"

#include "Libraries/glm/glm.hpp"

#include <vector>

class GridCell {
public:
	GridCell(int x0, int y0, int z0, glm::vec3 origin0, glm::vec3 center0);
	~GridCell();

	ObjectIdType GetFirstOccupantId();
	ObjectIdType GetOccupantIdAtElevation(int elevation);
	ObjectIdType GetTopOccupantId(); // In other words, the last occupant in the list

	void SetFirstOccupantId(ObjectIdType id);
	void SetOccupantIdAtElevation(ObjectIdType id, int elevation);

	int x; // Grid cell coordinate x
	int y; // Ground level elevation
	int z; // Grid cell coordinate z
	glm::vec3 origin; // Southwest corner of the cell in world coordinates
	glm::vec3 center; // Center of the cell in world coordinates
	std::vector<ObjectIdType> unitIds; // List of units occupying the cell
	std::vector<ObjectIdType> staticObjs; // List of static object IDs that occupy the cell

	/*
	? Vision blocking
	? Cover
	? Illumination
	? Terrain Weight (for pathfinding)
	*/
};

#endif // GRIDCELL_H
