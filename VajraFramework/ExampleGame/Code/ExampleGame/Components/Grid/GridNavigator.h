//
//  GridNavigator.h
//  Created by Matt Kaufmann on 12/12/13.
//

#ifndef GRIDNAVIGATOR_H
#define GRIDNAVIGATOR_H

#include "ExampleGame/Components/Grid/GridCell.h"

#include "Libraries/glm/glm.hpp"

#include "Vajra/Common/Components/Component.h"

#include <list>

class GridNavigator : public Component {
public:
	GridNavigator();
	GridNavigator(Object* object_);
	~GridNavigator();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	inline GridCell* GetCurrentCell()          { return this->currentCell;    }

	inline void SetMovementSpeed(float speed)  { this->movementSpeed = speed; }
	inline void SetCurrentCell(GridCell* cell) { this->currentCell = cell;    }

	void SetGridPosition(int x, int z);  // Place the object at the indicated cell on the grid.

	// Each of the following methods return true if a valid path can be found, false otherwise.
	bool SetDestination(int x, int z);   // Set destination to the center of the designated cell
	bool SetDestination(glm::vec3 loc);  // Set destination to the designated world position
	bool AddDestination(int x, int z);   // Set new destination without destroying current path
	bool AddDestination(glm::vec3 loc);  // Set new destination without destroying current path

private:
	void init();
	void destroy();

	void update();

	void followPath();

	void changeCell(GridCell* goalCell);

	float calculatePath(GridCell* startCell, GridCell* goalCell, std::list<GridCell*>& outPath); // Calculates a path and returns its length. Returns -1 if no path found.
	float travelCostEstimate(GridCell* startCell, GridCell* goalCell); // Estimated distance between two cells
	float actualTravelCost(GridCell* startCell, GridCell* goalCell);
	void simplifyPath(std::list<GridCell*>& outPath);

	static ComponentIdType componentTypeId;

	GridCell* currentCell;
	std::list<GridCell*> currentPath;
	bool isTraveling;
	float movementSpeed;
};

#endif // GRIDNAVIGATOR_H
