//
//  Guard.h
//  Created by Matt Kaufmann on 02/06/14.
//

#ifndef GUARD_H
#define GUARD_H

#include "ExampleGame/Components/GameScripts/Units/EnemyUnit.h"

#include <list>
#include <string>

//[[COMPONENT]]//
class Guard : public EnemyUnit {
public:
	Guard();
	Guard(Object* object_);
	~Guard();

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	virtual bool CanBeKilledBy(ObjectIdType id, glm::vec3 source);

protected:
	virtual void determineBrainState();

	virtual void cautiousUpdate();
	virtual void aggressiveUpdate();

	virtual void onBrainBecameCalm();
	virtual void onBrainBecameCautious();
	virtual void onBrainBecameAggressive();

	virtual void onSightedPlayerUnit(ObjectIdType id);
	virtual void onLostSightOfPlayerUnit(ObjectIdType id);

	virtual void onAnimationEnded(std::string animName);

private:
	void init();
	void destroy();

	void updateAlertness();

	int numPlayerUnitsSpotted;
	std::list<ObjectIdType> knownPlayers;
	float cooldownTimer;
	float attackTimer;  // Cooldown time between attacks
	bool isTargetInRange;
	//ObjectIdType targetId;
	GridCell* targetCell;
};

#endif // GUARD_H
