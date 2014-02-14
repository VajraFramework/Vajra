//
//  TransitionZone.h
//  Created by Matt Kaufmann on 01/31/14.
//

#ifndef TRANSITIONZONE_H
#define TRANSITIONZONE_H

#include "ExampleGame/Components/Grid/GridZone.h"
#include "Libraries/glm/glm.hpp"

// Forward declarations
class GridCell;

//[[COMPONENT]]//
class TransitionZone : public GridZone {
public:
	TransitionZone();
	TransitionZone(Object* object_);
	~TransitionZone();

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	// @Override
	//[[PROPERTY]]//
	virtual void SetZoneBounds(int xMin, int zMin, int xMax, int zMax);

	//[[PROPERTY]]//
	void SetFirstDestination(int cellX, int cellZ);
	void SetFirstDestination(glm::vec3 worldPosition);
	void SetFirstDestination(GridCell* cell);

	//[[PROPERTY]]//
	void SetSecondDestination(int cellX, int cellZ);
	void SetSecondDestination(glm::vec3 worldPosition);
	void SetSecondDestination(GridCell* cell);

	//[[PROPERTY]]//
	void SetDestinations(int cellX1, int cellZ1, int cellX2, int cellZ2);
	void SetDestinations(glm::vec3 worldPosition1, glm::vec3 worldPosition2);
	void SetDestinations(GridCell* cell1, GridCell* cell2);

private:
	void init();
	void destroy();

	void onUnitEnteredZone(ObjectIdType id);

	GridCell* destCell1;  // Initial destination
	GridCell* destCell2;  // Secondary destination
};

#endif // TRANSITIONZONE_H
