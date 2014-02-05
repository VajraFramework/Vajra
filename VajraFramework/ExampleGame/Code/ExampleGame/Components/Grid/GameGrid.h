//
//  GameGrid.h
//  Created by Matt Kaufmann on 02/04/14.
//

#ifndef GAMEGRID_H
#define GAMEGRID_H

#include "Libraries/glm/glm.hpp"
#include "Vajra/Common/Objects/Declarations.h"

#include <list>
#include <vector>

// Forward declarations
class GridCell;
class GridRoom;

class GameGrid {
public:
	GameGrid();
	~GameGrid();

	GridCell* GetCell(int gridX, int gridZ);     // Returns the cell at the specified coordinates
	GridCell* GetCell(glm::vec3 worldPosition);  // Returns the cell at the specified world position
	GridRoom* GetRoom(int gridX, int gridZ);     // Returns the room at the specified coordinates
	GridRoom* GetRoom(glm::vec3 worldPosition);  // Returns the room at the specified world position
	GridRoom* GetRoom(GridCell* cell);           // Returns the room at the specified grid cell

	// Populates the list with all adjacent cells
	void GetNeighbors(std::list<GridCell*>& outNbrs, GridCell* cell, int range = 1);

	void TouchedCells(GridCell* startCell, GridCell* goalCell, std::list<GridCell*>& outTouched);

	bool HasLineOfSight(GridCell* sourceCell, GridCell* targetCell);
	bool HasLineOfSight(int sourceCellX, int sourceCellZ, int targetCellX, int targetCellZ);

private:
	void init();
	void destroy();

	bool isWithinGrid(int cellX, int cellZ); // Returns true if the specified cell falls within the grid boundaries
	bool isWithinGrid(glm::vec3 loc); // Returns true if the vector position falls within a defined cell

	unsigned int gridWidth;
	unsigned int gridHeight;

	GridCell** gridCells;
	unsigned char** passableBits;
	unsigned char** visibleBits;

	std::vector<GridRoom*> gridRooms;
	std::list<ObjectIdType> gridZones;

	friend class GridManager;
};

#endif // GAMEGRID_H
