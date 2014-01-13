//
//  BaseUnit.h
//  ExampleGame
//

#ifndef BASE_UNIT_H
#define BASE_UNIT_H

#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "Vajra/Engine/Components/DerivedComponents/GameScript/GameScript.h"

class GridNavigator;
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

#endif // BASE_UNIT_H
