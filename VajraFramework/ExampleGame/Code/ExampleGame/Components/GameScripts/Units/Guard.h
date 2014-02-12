//
//  Guard.h
//  Created by Matt Kaufmann on 02/06/14.
//

#ifndef GUARD_H
#define GUARD_H

#include "ExampleGame/Components/GameScripts/Units/EnemyUnit.h"

#include <list>

//[[COMPONENT]]//
class Guard : public EnemyUnit {
public:
	Guard();
	Guard(Object* object_);
	~Guard();

protected:
	virtual void determineBrainState();
	virtual void setBrainState(EnemyBrainState bState);

	//virtual void idleUpdate();
	virtual void cautiousUpdate();
	virtual void aggressiveUpdate();

	virtual void onSightedPlayerUnit(ObjectIdType id);
	virtual void onLostSightOfPlayerUnit(ObjectIdType id);

private:
	void init();
	void destroy();

	void updateAlertness();

	int numPlayerUnitsSpotted;
	std::list<ObjectIdType> knownPlayers;
	float cooldownTimer;
	float attackTimer;  // Cooldown time between attacks
	bool isTargetInRange;
	ObjectIdType targetId;
};

#endif // GUARD_H
