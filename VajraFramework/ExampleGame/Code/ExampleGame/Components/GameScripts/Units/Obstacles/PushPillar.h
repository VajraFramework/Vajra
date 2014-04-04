//
//  PushPillar.h
//  Created by Matt Kaufmann on 03/31/14.
//

#ifndef PUSHPILLAR_H
#define PUSHPILLAR_H

#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"

//[[COMPONENT]]//
class PushPillar : public BaseUnit {
public:
	PushPillar();
	PushPillar(Object* object_);
	~PushPillar();

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	virtual bool CanBeKilledBy(ObjectIdType id, glm::vec3 source);

	static inline ComponentIdType GetTypeId()  { return BaseUnit::GetTypeId(); }

protected:
	virtual void start();
	virtual void end();
	virtual void update();

	virtual void onUnitSpecialHit(ObjectIdType id, int gridX, int gridZ, glm::vec3 source);

	virtual void Kill();

private:
	void init();
	void destroy();

	//void moveToBlockInDirection(glm::vec3 dir);
	void moveToBlockInDirection();
	void onZoneEnteredCell(int gridX, int gridZ);
	void onZoneExitedCell(int gridX, int gridZ);

	bool isSliding;
	glm::vec3 slideDir;
};

#endif // PUSHPILLAR_H
