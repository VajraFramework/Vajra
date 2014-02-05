//
//  GridCell.cpp
//  Created by Matt Kaufmann on 12/10/13.
//

#include "ExampleGame/Components/Grid/GridCell.h"

GridCell::GridCell() :
	x(0),
	z(0)
{ }

GridCell::GridCell(int x0, int y0, int z0, glm::vec3 origin0, glm::vec3 center0, bool passable0) :
	x(x0),
	y(y0),
	z(z0),
	origin(origin0),
	center(center0),
	isPassable(passable0),
	unitId(OBJECT_ID_INVALID)
{ }
