//
//  GridManager.h
//  Created by Matt Kaufmann on 12/10/13.
//

#ifndef GRIDMANAGER_H
#define GRIDMANAGER_H

#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "ExampleGame/Components/Grid/GameGrid.h"
#include "Libraries/glm/glm.hpp"
#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Input/ITouchTarget.h"
#include "Vajra/Utilities/MathUtilities.h"
#include "Vajra/Utilities/XmlParser/XmlParser.h"

#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>

class ShadyCamera;
//[[COMPONENT]]//
class GridManager : public Component, public IGameTouchTarget {
public:
	GridManager();
	GridManager(Object* object_);
	~GridManager();

	static inline unsigned int GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	inline GameGrid* GetGrid()  { return this->grid; }

	ObjectIdType GetPlayerUnitIdOfType(UnitType uType);
	inline ObjectIdType GetSelectedUnitId() { return this->selectedUnitId; }
	void SwitchSelectedUnit();
	// @Override
	virtual bool TestTouchStart(Touch /* touch */) { return true; }
	virtual void OnTouchUpdate(int touchIndex);
	
	GridCell* TouchPositionToCell(glm::vec2 touchPos);
	glm::vec3 TouchPositionToGridPositionAtElevation(glm::vec2 touchPos, int elevation);

	inline void SetShadyCamera(ShadyCamera* shadyCam) { this->shadyCamRef = shadyCam; }
	inline ShadyCamera* GetShadyCamera() { return this->shadyCamRef; }
	/****************
	// TODO [Implement]
	std::list<GridCell> GetNeighbors(GridCell* cel, bool diagonals, bool sameRoom);
	std::list<GridCell> GetNeighborsInRange(glm::vec3 pos, int range, bool includeObstructed, bool lineOfSight, bool sameElevation, GRID_DIR dir);
	void TouchOnGrid(uTouch uT); // Event that fires when a touch input is detected
	****************/

/******************************************************************************/

private:
	void init();
	void destroy();

	void onLevelLoaded();
	void onLevelUnloaded();
	
#ifdef DEBUG
	void debugTouchUpdate(int touchIndex);
#endif

	void loadGridDataFromXml(XmlNode* gridNode);
	void loadCellDataFromXml(XmlNode* cellDataNode);
	void loadRoomDataFromXml(XmlNode* roomDataNode);
	//void loadZoneDataFromXml(XmlNode* zoneDataNode);

	void placeStaticObjectOnGrid(ObjectIdType id, int westBound, int southBound, int width, int height);
	void placeUnitOnGrid(ObjectIdType id, int cellX, int cellZ);
	void placeZoneOnGrid(ObjectIdType id);
	void gridCellChangedHandler(ObjectIdType id, int gridX, int gridZ, int elevation);
	void removeNavigatorFromGrid(ObjectIdType id, int elevation);
	bool checkUnitCollisions(ObjectIdType id, GridCell* destCell, int elevation);
	void checkZoneCollisions(ObjectIdType id, GridCell* startCell, GridCell* destCell);
	void checkRoomCollisions(ObjectIdType id, GridCell* startCell, GridCell* destCell);

	void tryUnitSwitch(int touchIndex);
	void selectUnitInCell(int x, int z);
	void selectUnitInCell(GridCell* cell);
	void deselectUnit();
	void longPressOnGrid();

	GameGrid* grid;
	ShadyCamera* shadyCamRef;
	int maxElevation;
	Plane gridPlane;  // The center of cell (0,0) in world coordinates and it's normal

	std::map<UnitType, ObjectIdType> playerUnits;
	ObjectIdType selectedUnitId;

	//GameObject* transZones;

	/****************
	int offsetX; // X-coordinate of leftmost room
	int offsetZ; // Z-coordinate of southmost room

	Plane* plane; // Plane for raycasting against the ground
	Plane* raisedPlane; // Plane for raycasting against the raised elevation

	std::list<GridCell*> occupiedCells;

	std::list<PlayerUnit*> pUnits;
	****************/

	static unsigned int componentTypeId;

	friend class LevelLoader;
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
