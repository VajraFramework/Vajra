//
//  GridCell.h
//  Created by Matt Kaufmann on 12/10/13.
//

#ifndef GRIDCELL_H
#define GRIDCELL_H

#include "Vajra/Engine/GameObject/GameObject.h"

#include "Libraries/glm/glm.hpp"

class GridCell {
public:
	GridCell(int x0, int y0, int z0, glm::vec3 origin0, glm::vec3 center0, bool passable0);

	int x; // Grid cell coordinate x
	int y; // Elevation
	int z; // Grid cell coordinate z
	glm::vec3 origin; // Southwest corner of the cell in world coordinates
	glm::vec3 center; // Center of the cell in world coordinates
	bool isPassable; // If false, units will not attempt to navigate through the cell
	GameObject* occupant; // The object in the cell

	/*
	? Vision blocking
	? Cover
	? Illumination
	? Terrain Weight (for pathfinding)
	*/
};

#endif // GRIDCELL_H
