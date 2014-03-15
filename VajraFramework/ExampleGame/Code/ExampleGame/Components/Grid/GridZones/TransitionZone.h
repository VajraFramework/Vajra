//
//  TransitionZone.h
//  Created by Matt Kaufmann on 01/31/14.
//

#ifndef TRANSITIONZONE_H
#define TRANSITIONZONE_H

#include "ExampleGame/Components/Grid/GridZone.h"
#include "Libraries/glm/glm.hpp"

class GridCell;

//[[COMPONENT]]//
class TransitionZone : public GridZone {
public:
	TransitionZone();
	TransitionZone(Object* object_);
	~TransitionZone();

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	void GetFirstDestinationCoordinates(int& cellX, int& cellZ);
	void GetSecondDestinationCoordinates(int& cellX, int& cellZ);
	void GetBothDestinationCoordinates(int& cellX1, int& cellZ1, int& cellX2, int& cellZ2);

	// @Override
	//[[PROPERTY]]//
	virtual void SetZoneBounds(int xMin, int zMin, int xMax, int zMax);

	//[[PROPERTY]]//
	void SetFirstDestination(int cellX, int cellZ);

	//[[PROPERTY]]//
	void SetSecondDestination(int cellX, int cellZ);

	//[[PROPERTY]]//
	void SetDestinations(int cellX1, int cellZ1, int cellX2, int cellZ2);

private:
	void init();
	void destroy();

	void onUnitEnteredZone(ObjectIdType id, GridCell* startCell);

	int destX1, destZ1;	// Coordinates of first destination relative to object's position
	int destX2, destZ2; // Coordinates of second destination relative to object's position
};

#endif // TRANSITIONZONE_H
