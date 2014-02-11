//
//  GameGrid.h
//  Created by Matt Kaufmann on 02/04/14.
//

#ifndef GAMEGRID_H
#define GAMEGRID_H

#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridConstants.h"
#include "ExampleGame/Components/Grid/GridRoom.h"
#include "ExampleGame/Components/Grid/GridZone.h"
#include "Libraries/glm/glm.hpp"
#include "Vajra/Common/Objects/Declarations.h"

#include <list>
#include <vector>

class GameGrid {
public:
	GameGrid(unsigned int spanX, unsigned int spanZ);
	~GameGrid();

	int GetElevationFromWorldY(float worldY);

	inline unsigned int GetGridWidth()   { return this->gridWidth;  }
	inline unsigned int GetGridHeight()  { return this->gridHeight; }

	GridCell* GetCell(int gridX, int gridZ);     // Returns the cell at the specified coordinates
	GridCell* GetCell(glm::vec3 worldPosition);  // Returns the cell at the specified world position

	// Populates the list with all adjacent cells and returns the number of adjacent cells
	void GetNeighborCells(std::list<GridCell*>& outNbrs, GridCell* cell, float range = 1.0f);

	void AddGridRoom(int west, int south, int width, int height);

	GridRoom* GetRoom(int gridX, int gridZ);     // Returns the room at the specified coordinates
	GridRoom* GetRoom(glm::vec3 worldPosition);  // Returns the room at the specified world position
	GridRoom* GetRoom(GridCell* cell);           // Returns the room at the specified grid cell

	// returns the world position of the center of a room
	glm::vec3 GetRoomCenter(int x, int z);
	glm::vec3 GetRoomCenter(GridCell* cell);

	void AddGridZone(ObjectIdType zoneId);

	GridZone* GetZone(int gridX, int gridZ);     // Returns the first zone the cell at the specified coordinates resides in
	GridZone* GetZone(glm::vec3 worldPosition);  // Returns the first zone the cell at the specified world position resides in
	GridZone* GetZone(GridCell* cell);           // Returns the first zone the specified cell resides in

	void TouchedCells(GridCell* startCell, GridCell* goalCell, std::list<GridCell*>& outTouched);

	bool Passable(GridCell* startCell, GridCell* goalCell);

	bool HasLineOfSight(GridCell* sourceCell, GridCell* targetCell);
	bool HasLineOfSight(int sourceCellX, int sourceCellZ, int targetCellX, int targetCellZ);

	int GetCellGroundLevel(int gridX, int gridZ);
	bool IsCellPassableAtElevation(int gridX, int gridZ, unsigned int elevation);
	bool IsCellVisibleAtElevation(int gridX, int gridZ, unsigned int elevation);

	void SetCellGroundLevel(int gridX, int gridZ, unsigned int elevation);
	void SetCellPassableAtElevation(int gridX, int gridZ, unsigned int elevation, bool isPassable);
	void SetCellVisibleAtElevation(int gridX, int gridZ, unsigned int elevation, bool isVisible);

private:
	void init(unsigned int spanX, unsigned int spanZ);
	void destroy();

	bool isWithinGrid(int cellX, int cellZ);     // Returns true if the specified cell falls within the grid boundaries
	bool isWithinGrid(glm::vec3 worldPosition);  // Returns true if the vector position falls within a defined cell

	unsigned int gridWidth;
	unsigned int gridHeight;

	GridCell*** gridCells;
	unsigned char*** passableBits;
	unsigned char*** visibleBits;

	std::vector<GridRoom*> gridRooms;
	std::list<ObjectIdType> gridZones;

#ifdef DEBUG
public:
	void debugDrawGrid();

private:
	//Color gridColor; // Default debug grid color
	//Color gridColor4; // Color used for every fourth grid cell
	//Color gridColor16; // Color used for every sixteenth grid cell
	//Color selectionColor; // Color used for the selected cell
	//Color pathColor; // Color used to draw the path
	bool paintSolid; // If true, cells are painted as solid rather than wireframe
#endif
};

#endif // GAMEGRID_H
