//
//  EnemyUnit.h
//  Created by Matt Kaufmann on 01/14/14.
//

#ifndef ENEMYUNIT_H
#define ENEMYUNIT_H

#include "ExampleGame/Components/GameScripts/Ai/AiKnowledge.h"
#include "ExampleGame/Components/GameScripts/Ai/AiRoutine.h"
#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"

class EnemyUnit : public BaseUnit {
public:
	EnemyUnit();
	EnemyUnit(Object* object_);
	~EnemyUnit();

	void Activate();

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

protected:
	virtual void update();

	virtual void determineBrainState() = 0;

	virtual void idleUpdate();
	virtual void cautiousUpdate();
	virtual void aggressiveUpdate();

	virtual void onSightedPlayerUnit(ObjectIdType /*id*/) { };
	virtual void onLostSightOfPlayerUnit(ObjectIdType /*id*/) { };

	AiRoutine* routine;
	float alertness;
	EnemyBrainState brainState;

private:
	void init();
	void destroy();

	AiKnowledge* knowledge;
	//GridNavigator* navigator;

	bool isActive;
};

#endif // ENEMYUNIT_H
