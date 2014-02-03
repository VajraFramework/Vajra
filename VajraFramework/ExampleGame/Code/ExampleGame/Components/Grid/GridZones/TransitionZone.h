//
//  TransitionZone.h
//  Created by Matt Kaufmann on 01/31/14.
//

#ifndef TRANSITIONZONE_H
#define TRANSITIONZONE_H

#include "ExampleGame/Components/Grid/GridZone.h"
#include "Libraries/glm/glm.hpp"

//[[COMPONENT]]//
class TransitionZone : public GridZone {
public:
	TransitionZone();
	TransitionZone(Object* object_);
	~TransitionZone();

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	void SetDestination(int cellX, int cellZ);
	void SetDestination(glm::vec3 worldPos);

private:
	void init();
	void destroy();

	void onUnitEnteredZone(ObjectIdType id);

	int destX, destZ; // Units that enter this zone are sent to these coordinates
};

#endif // TRANSITIONZONE_H
