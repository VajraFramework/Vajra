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

glm::vec3 GridRoom::GetCenter() {
	glm::vec3 center;
	center.x = (this->westBound + this->eastBound + 1) / 2.0f;
	center.y = 0.0f;
	center.z = (this->southBound + this->northBound + 1) / 2.0f;
	return center;
}
