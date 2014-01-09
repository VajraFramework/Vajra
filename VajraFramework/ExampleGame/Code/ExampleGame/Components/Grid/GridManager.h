//
//  GridManager.h
//  Created by Matt Kaufmann on 12/10/13.
//

#ifndef GRIDMANAGER_H
#define GRIDMANAGER_H

#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridRoom.h"
#include "ExampleGame/Components/Grid/GridZone.h"
#include "Libraries/glm/glm.hpp"
#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Input/ITouchTarget.h"
#include "Vajra/Utilities/MathUtilities.h"

#include <list>
#include <string>
#include <vector>

class GridManager : public Component, public IGameTouchTarget {
public:
	GridManager();
	GridManager(Object* object_);
	~GridManager();

	static inline unsigned int GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	void GenerateTerrainFromFile(std::string terrainFilename);
	void AddGridZone(ObjectIdType zoneId);

	inline float getCellSize() { return cellSize; }

	GridCell* GetCell(int x, int z);    // Returns the cell at the specified coordinates
	GridCell* GetCell(glm::vec3 loc);   // Returns the cell at the specified world position
	GridRoom* GetRoom(int x, int z);    // Returns the room at the specified coordinates
	GridRoom* GetRoom(glm::vec3 loc);   // Returns the room at the specified world position
	GridRoom* GetRoom(GridCell* cell);  // Returns the room at the specified grid cell

	void GetNeighbors(GridCell* cel, std::list<GridCell*>& outNbrs); // Populates the list with all adjacent cells

	// returns the world position of the center of a room
	glm::vec3 GetRoomCenter(int x, int z);
	glm::vec3 GetRoomCenter(GridCell* cell);

	// @Override
	virtual bool TestTouch(int /*touchIndex*/) { return true; }
	virtual void OnTouch(int touchIndex);
	
	GridCell* TouchPositionToCell(glm::vec2 touchPos);
	glm::vec3 TouchPositionToGridPosition(glm::vec2 touchPos);

	bool Passable(GridCell* startCell, GridCell* goalCell);
	void TouchedCells(GridCell* startCell, GridCell* goalCell, std::list<GridCell*>& outTouched);

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
	void DebugTouchTest(int touchIndex);
#endif

	void gridCellChangedHandler(ObjectIdType id, glm::vec3 dest);
	void checkZoneCollisions(ObjectIdType id, GridCell* startCell, GridCell* destCell);

	float cellSize; // Width and depth of a grid cell in world coordinates
	glm::vec3 halfCellSize; // Offset vector between center and corner of a grid cell

	GridCell*** gridCells; // 2D array of grid cells
	std::vector<GridRoom*> gridRooms;
	std::list<ObjectIdType> gridZones;

	unsigned int gridWidth;
	unsigned int gridHeight;
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
****************/

#endif // GRIDMANAGER_H
