//
//  EnemyUnit.h
//  Created by Matt Kaufmann on 01/14/14.
//

#ifndef ENEMYUNIT_H
#define ENEMYUNIT_H

#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"

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
};

#endif // ENEMYUNIT_H
