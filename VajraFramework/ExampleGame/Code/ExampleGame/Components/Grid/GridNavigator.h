//
//  GridNavigator.h
//  Created by Matt Kaufmann on 12/12/13.
//

#ifndef GRIDNAVIGATOR_H
#define GRIDNAVIGATOR_H

#include "ExampleGame/Components/Grid/GridCell.h"

#include "Vajra/Common/Components/Component.h"

#include <list>

class GridNavigator : public Component {
public:
	GridNavigator();
	GridNavigator(Object* object_);
	~GridNavigator();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(Message* message);

private:
	void init();
	void destroy();

	void update();

	float calculatePath(GridCell* startCell, GridCell* goalCell, std::list<GridCell*>& outPath); // Calculates a path and returns its length. Returns -1 if no path found.
	float travelCostEstimate(GridCell* startCell, GridCell* goalCell); // Estimated distance between two cells
	float actualTravelCost(GridCell* startCell, GridCell* goalCell);

	static ComponentIdType componentTypeId;
};

#endif // GRIDNAVIGATOR_H
