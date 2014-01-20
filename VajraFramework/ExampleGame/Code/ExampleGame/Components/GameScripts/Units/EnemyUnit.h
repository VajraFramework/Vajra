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

//[[COMPONENT]]//
class EnemyUnit : public BaseUnit {
public:
	EnemyUnit();
	EnemyUnit(Object* object_);
	~EnemyUnit();

protected:
	virtual void update();

private:
	void init();
	void destroy();

	AiKnowledge* knowledge;
	AiRoutine* routine;
	GridNavigator* navigator;
};

#endif // ENEMYUNIT_H
