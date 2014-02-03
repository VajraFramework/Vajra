//
//  GridRoom.h
//  Created by Matt Kaufmann on 01/08/14.
//

#ifndef GRIDROOM_H
#define GRIDROOM_H

#include "Libraries/glm/glm.hpp"

class GridRoom {
public:
	GridRoom(int west, int south, int width, int height);

	inline int GetWidth()   { return 1 + eastBound - westBound;   }
	inline int GetHeight()  { return 1 + northBound - southBound; }
	glm::vec3 GetCenter();

	inline bool IsWithinRoom(int x, int z) { return (x >= westBound) && (x <= eastBound) && (z >= southBound) && (z <= northBound); }

	int westBound;
	int eastBound;
	int southBound;
	int northBound;
};

#endif // GRIDROOM_H
