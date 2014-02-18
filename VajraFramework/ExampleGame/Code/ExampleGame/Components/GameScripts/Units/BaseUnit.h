//
//  BaseUnit.h
//  ExampleGame
//

#ifndef BASE_UNIT_H
#define BASE_UNIT_H

#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "Vajra/Common/Components/Component.h"

// Forward Declarations:
class GridNavigator;
class GameObject;

// Not exposing this as a Component that can be added via XML
class BaseUnit : public Component {
public:
	BaseUnit();
	BaseUnit(Object* object_);
	~BaseUnit();

	inline UnitState GetUnitState() { return this->unitState; }
	inline UnitType GetUnitType() { return this->unitType; }
	inline UnitActionState GetUnitActionState() { return this->unitActionState; }

	void SwitchActionState(UnitActionState newState);

	static inline ComponentIdType GetTypeId()  { return componentTypeId; }

protected:
	virtual void start();
	virtual void end();
	virtual void update();

	void Kill(); 

	UnitType unitType;

	GameObject* gameObjectRef;
	GridNavigator* gridNavRef;

private:
	void init();
	void destroy();

	UnitState unitState;
	UnitActionState unitActionState;

	static ComponentIdType componentTypeId;
};

#endif // BASE_UNIT_H
