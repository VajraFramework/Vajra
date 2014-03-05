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
	float ConvertElevationToWorldY(unsigned int elevation);

	inline unsigned int GetGridWidth()   { return this->gridWidth;  }
	inline unsigned int GetGridHeight()  { return this->gridHeight; }

	void GetCoordinates(int& outX, int& outZ, glm::vec3 worldPosition);

	GridCell* GetCell(int gridX, int gridZ);     // Returns the cell at the specified coordinates
	GridCell* GetCell(glm::vec3 worldPosition);  // Returns the cell at the specified world position

	// Returns number of cell "hops" to get from start to goal
	int GetCellDistanceBetweenCells(int startCellX, int startCellZ, int goalCellX, int goalCellZ);
	int GetCellDistanceBetweenCells(GridCell* startCell, GridCell* goalCell);

	// Returns distance along the ground plane from start to goal
	float GetGroundDistanceBetweenCells(int startCellX, int startCellZ, int goalCellX, int goalCellZ);
	float GetGroundDistanceBetweenCells(GridCell* startCell, GridCell* goalCell);

	// Returns 3-D distance from start to goal
	float GetTrueDistanceBetweenCells(int startCellX, int startCellZ, int goalCellX, int goalCellZ);
	float GetTrueDistanceBetweenCells(GridCell* startCell, GridCell* goalCell);

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
	bool ZoneExistsOnGrid(ObjectIdType zoneId);

	GridZone* GetZone(int gridX, int gridZ);     // Returns the first zone the cell at the specified coordinates resides in
	GridZone* GetZone(glm::vec3 worldPosition);  // Returns the first zone the cell at the specified world position resides in
	GridZone* GetZone(GridCell* cell);           // Returns the first zone the specified cell resides in

	void GetZones(std::list<GridZone*>& outZones, int gridX, int gridZ);
	void GetZones(std::list<GridZone*>& outZones, glm::vec3 worldPosition);
	void GetZones(std::list<GridZone*>& outZones, GridCell* cell);

	void TouchedCells(glm::vec3 startPosition, glm::vec3 goalPosition, std::list<GridCell*>& outTouched);
	void TouchedCells(GridCell* startCell, GridCell* goalCell, std::list<GridCell*>& outTouched); 

	bool Passable(GridCell* startCell, GridCell* goalCell);

	bool HasLineOfSight(GridCell* sourceCell, GridCell* targetCell);
	bool HasLineOfSight(int sourceCellX, int sourceCellZ, int targetCellX, int targetCellZ);

	bool HasLineOfSight(GridCell* sourceCell, GridCell* targetCell, unsigned int elevation);
	bool HasLineOfSight(int sourceCellX, int sourceCellZ, int targetCellX, int targetCellZ, unsigned int elevation);

	int GetCellGroundLevel(int gridX, int gridZ);
	bool IsCellPassableAtElevation(int gridX, int gridZ, unsigned int elevation);
	bool IsCellVisibleAtElevation(int gridX, int gridZ, unsigned int elevation);

	void ChangeCellGroundLevel(int gridX, int gridZ, int elevationDiff);
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
