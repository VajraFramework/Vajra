//
//  Guard.h
//  Created by Matt Kaufmann on 02/06/14.
//

#ifndef GUARD_H
#define GUARD_H

#include "ExampleGame/Components/GamEScripts/Units/EnemyUnit.h"

#include <list>

//[[COMPONENT]]//
class Guard : public EnemyUnit {
public:
	Guard();
	Guard(Object* object_);
	~Guard();

protected:
	virtual void determineBrainState();

	//virtual void idleUpdate();
	virtual void cautiousUpdate();
	virtual void aggressiveUpdate();

	virtual void onSightedPlayerUnit(ObjectIdType id);
	virtual void onLostSightOfPlayerUnit(ObjectIdType id);

private:
	void init();
	void destroy();

	int numPlayerUnitsSpotted;
	std::list<ObjectIdType> knownPlayers;
};

#endif // GUARD_H
