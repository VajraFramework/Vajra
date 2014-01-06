//
//  GridManager.h
//  Created by Matt Kaufmann on 12/10/13.
//

#ifndef GRIDMANAGER_H
#define GRIDMANAGER_H

#include "ExampleGame/Components/Grid/GridCell.h"
#include "Libraries/glm/glm.hpp"
#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Utilities/MathUtilities.h"

#include <string>

class GridManager : public Component {
public:
	GridManager();
	GridManager(Object* object_);
	~GridManager();

	static inline unsigned int GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	void GenerateTerrainFromFile(std::string terrainFilename);

	int GetRoomX(int cellX); // Returns the room in which the cell resides
	int GetRoomZ(int cellZ); // Returns the room in which the cell resides

	inline int GetRoomWidth() { return this->roomWidth; }
	inline int GetRoomHeight() { return this->roomHeight; }

	inline float getCellSize() { return cellSize; }

	GridCell* GetCell(int x, int z); // Returns the cell at the specified coordinates
	GridCell* GetCell(glm::vec3 loc); // Returns the cell at the specified world position

	// returns the world position of the center of a room
	glm::vec3 GetRoomCenter(int x, int z);
	glm::vec3 GetRoomCenter(GridCell* cell);
	
	GridCell* TouchPositionToCell(glm::vec2 touchPos);
	glm::vec3 TouchPositionToGridPosition(glm::vec2 touchPos);

	/****************
	// TODO [Implement]
	std::list<GridCell> GetNeighbors(GridCell* cel, bool diagonals, bool sameRoom);
	std::list<GridCell> GetNeighborsInRange(glm::vec3 pos, int range, bool includeObstructed, bool lineOfSight, bool sameElevation, GRID_DIR dir);
	bool HasLineOfSight(int startX, int startZ, int endX, int endZ);
	std::list<GridCell> DirectRoute(int startX, int startZ, int endX, int endZ);
	void TouchOnGrid(uTouch uT); // Event that fires when a touch input is detected
	****************/

/******************************************************************************/

private:
	void init();
	void destroy();

	bool IsWithinGrid(int cellX, int cellZ); // Returns true if the specified cell falls within the grid boundaries
	bool IsWithinGrid(glm::vec3 loc); // Returns true if the vector position falls within a defined cell
#ifdef DEBUG
	void DebugDrawGrid();
	void DebugTouchTest();
#endif
	float cellSize; // Width and depth of a grid cell in world coordinates
	glm::vec3 halfCellSize; // Offset vector between center and corner of a grid cell

	GridCell*** gridCells; // 2D array of grid cells

	unsigned int gridWidth;
	unsigned int gridHeight;
	unsigned int roomWidth; // Width of an individual room in grid cells
	unsigned int roomHeight; // Height of an individual room in grid cells
	int roomOffsetX; // Number of cells separating rooms horizontally
	int roomOffsetZ; // Number of cells separating rooms vertically
	int maxElevation;
	Plane gridPlane;  // The center of cell (0,0) in world coordinates and it's normal

	//GameObject* transZones;

	/****************
	int offsetX; // X-coordinate of leftmost room
	int offsetZ; // Z-coordinate of southmost room

	Plane* plane; // Plane for raycasting against the ground
	Plane* raisedPlane; // Plane for raycasting against the raised elevation

	std::list<GridCell*> occupiedCells;

	std::list<PlayerUnit*> pUnits;
	****************/

#ifdef DEBUG
	//Color gridColor; // Default debug grid color
	//Color gridColor4; // Color used for every fourth grid cell
	//Color gridColor16; // Color used for every sixteenth grid cell
	//Color selectionColor; // Color used for the selected cell
	//Color pathColor; // Color used to draw the path
	bool paintSolid; // If true, cells are painted as solid rather than wireframe
#endif

	static unsigned int componentTypeId;

};

/****************
// TODO [Cleanup] These should be moved into other classes
void NextUnit();
void SetUnitIcon();
void UpdateUnitIcon(float mod);
std::string GetUnitName(UnitBase.UnitType pU);
void InitUnits();

UnitBase.UnitType startUnit;
CameraController* cC;
OverviewCam* oV;
VIEW_MODE curViewMode;
VIEW_MODE { VIEW_MODE_GAME, VIEW_MODE_TRANSITION, VIEW_MODE_OVERVIEW }
/****************/

#endif // GRIDMANAGER_H
