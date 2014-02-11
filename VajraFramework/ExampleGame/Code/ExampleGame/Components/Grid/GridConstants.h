//
//  GridConstants.h
//  Created by Matt Kaufmann on 02/03/14.
//

#ifndef GRIDCONSTANTS_H
#define GRIDCONSTANTS_H

#include "Libraries/glm/glm.hpp"

#define CELL_SIZE 1.0f
#define GROUND_Y 0.0f
extern const glm::vec3 HALF_CELL;

#define ELEVATION_UNIT 2.0f
#define NUM_ELEVATIONS 3

#define ROOM_WIDTH_INDOORS 15
#define ROOM_HEIGHT_INDOORS 9
#define ROOM_WIDTH_OUTDOORS 17
#define ROOM_HEIGHT_OUTDOORS 11

#endif // GRIDCONSTANTS_H
