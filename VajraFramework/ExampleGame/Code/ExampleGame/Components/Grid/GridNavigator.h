//
//  GridNavigator.h
//  Created by Matt Kaufmann on 12/12/13.
//

#ifndef GRIDNAVIGATOR_H
#define GRIDNAVIGATOR_H

#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "Libraries/glm/glm.hpp"
#include "Vajra/Common/Components/Component.h"

#include <list>

//[[COMPONENT]]//
class GridNavigator : public Component {
public:
	GridNavigator();
	GridNavigator(Object* object_);
	~GridNavigator();

	static inline ComponentIdType GetTypeId()  { return componentTypeId;              }

	inline GridCell* GetCurrentCell()          { return this->currentCell;            }
	inline bool IsTraveling()                  { return this->isTraveling;            }
	inline bool IsTurning()                    { return this->isTurning;              }
	inline bool IsMoving()                     { return IsTraveling() || IsTurning(); }

	GridCell* GetDestination();

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	inline void SetMovementSpeed(float speed);
	//[[PROPERTY]]//
	inline void SetTurnSpeedDegrees(float degreesPerSecond);
	inline void SetTurnSpeedRadians(float /* radiansPerSecond */);
	inline void SetCurrentCell(GridCell* cell) { this->currentCell = cell;            }
	void SetIsTraveling(bool isTraveling_);
	void SetMaxNavigableUnitType(UnitType uType);

	//[[PROPERTY]]//
	void SetGridPosition(int x, int z);   // Place the object at the indicated cell on the grid.
	void SetGridPosition(glm::vec3 loc);  // Place the object at the cell at the indicated position
	void SetGridPosition(GridCell* cell); // Place the object at the center of the indicated cell

	// Each of the following methods return true if a valid path can be found, false otherwise.
	//[[PROPERTY]]//
	bool SetDestination(int x, int z, bool ignoreCellOccupants = false);    // Set destination to the center of the designated cell
	bool SetDestination(glm::vec3 loc, bool ignoreCellOccupants = false);   // Set destination to the designated world position
	bool SetDestination(GridCell* cell, bool ignoreCellOccupants = false);  // Set destination to the center of the designated cell
	//[[PROPERTY]]//
	bool AddDestination(int x, int z);    // Set new destination without destroying current path
	bool AddDestination(glm::vec3 loc);   // Set new destination without destroying current path
	bool AddDestination(GridCell* cell);  // Set new destination without destroying current path

	// The following methods can be used to circumvent path checking.
	void SetForcedDestination(int gridX, int gridZ);
	void SetForcedDestination(glm::vec3 worldPosition);
	void SetForcedDestination(GridCell* cell);

	void AddForcedDestination(int gridX, int gridZ);
	void AddForcedDestination(glm::vec3 worldPosition);
	void AddForcedDestination(GridCell* cell);

	void SetLookTarget(int x, int z);     // Set look target to the center of the designated cell
	void SetLookTarget(glm::vec3 loc);    // Set look target to the designated world position
	void SetLookTarget(glm::quat orient); // Set look target to the designated orientation
	void SetTargetForward(glm::vec3 forward);

	bool CanReachDestination(int cellX, int cellZ, float maxDistance = -1.0f, bool ignoreCellOccupants = false);
	bool CanReachDestination(glm::vec3 worldPos, float maxDistance = -1.0f, bool ignoreCellOccupants = false);
	bool CanReachDestination(GridCell* cell, float maxDistance = -1.0f, bool ignoreCellOccupants = false);

	void DisableNavigation();
	void EnableNavigation();
	void HaltMovement();

	void ReturnToCellCenter();  // Use this method if you want to stop a unit's movement but have it end at the center of a cell.

protected:
	virtual void start();
	virtual void update();

private:
	void init();
	void destroy();

	void recalculatePath();
	void followPath();
	void updateFacing();

	void changeCell(GridCell* goalCell);

	float calculatePath(GridCell* startCell, GridCell* goalCell, std::list<GridCell*>& outPath, bool ignoreCellOccupants = false); // Calculates a path and returns its length. Returns -1 if no path found.
	float travelCostEstimate(GridCell* startCell, GridCell* goalCell); // Estimated distance between two cells
	float actualTravelCost(GridCell* startCell, GridCell* goalCell);
	bool canNavigateThroughCellAtElevation(GridCell* cell, int elevation, bool ignoreCellOccupants = false);
	void simplifyPath(std::list<GridCell*>& outPath, bool ignoreCellOccupants = false);
	void setNextWaypoint();
	void goToNearestPassableCell();

	Transform* getTransform() { return this->gameObjectRef->GetTransform(); }

	GridCell* currentCell;
	std::list<GridCell*> currentPath;
	std::list<GridCell*> currentSegment;
	glm::vec3 targetForward;
	bool isTraveling;
	bool isTurning;
	float movementSpeed;
	float turningSpeed;
	UnitType maxNavigableUnitType;
	bool ignoreOccupantsForPathing; // This is stored each time we set a path in case the path needs to be recalculated.
	bool ignoreEverything;          // If the navigator somehow ends up inside of a blocked area, this will turn on
	bool navigationEnabled;

	GameObject* gameObjectRef;

	static ComponentIdType componentTypeId;
};

////////////////////////////////////////////////////////////////////////////////

// Inline Functions:
//
void GridNavigator::SetMovementSpeed(float speed) {
	this->movementSpeed = speed;
}

void GridNavigator::SetTurnSpeedDegrees(float degreesPerSecond) {
	this->turningSpeed = degreesPerSecond inRadians;
}

void GridNavigator::SetTurnSpeedRadians(float radiansPerSecond) {
	this->turningSpeed = radiansPerSecond;
}

#endif // GRIDNAVIGATOR_H
