//
//  GridRoom.cpp
//  Created by Matt Kaufmann on 01/08/14.
//

#include "ExampleGame/Components/Grid/GridRoom.h"

GridRoom::GridRoom(int west, int south, int width, int height) :
	westBound(west),
	eastBound(west + width - 1),
	southBound(south),
	northBound(south + height - 1)
{ }
