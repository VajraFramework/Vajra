//
//  BaseUnit.h
//  ExampleGame
//

#ifndef BASEUNIT_H
#define BASEUNIT_H

#include "Vajra/Engine/Components/DerivedComponents/GameScript/GameScript.h"

class GridNavigator;

enum UnitState {
	UNIT_STATE_ALIVE,
	UNIT_STATE_DEAD
};

enum UnitType {
	UNIT_TYPE_ASSASSIN,
	UNIT_TYPE_THIEF,
	UNIT_TYPE_IMPOSTOR,
	UNIT_TYPE_PLAYER_UNITS_COUNT,
	UNIT_TYPE_GUARD,
	UNIT_TYPE_TARGET,
	UNIT_TYPE_CAPTAIN
};

class BaseUnit : public GameScript {
public:
	BaseUnit();
	BaseUnit(Object* object_);
	~BaseUnit();

	inline UnitState GetUnitState() { return this->unitState; }
	inline UnitType GetUnitType() { return this->unitType; }

	static inline ComponentIdType GetTypeId()  { return componentTypeId; }

protected:
	virtual void start();
	virtual void end();
	virtual void update();

	void Kill();

	UnitType unitType;

private:
	void init();
	void destroy();

	GridNavigator* gridNavRef;

	UnitState unitState = UnitState::UNIT_STATE_ALIVE;

	static ComponentIdType componentTypeId;
};

#endif // BASEUNIT_H
