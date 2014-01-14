//
//  PlayerUnit.h
//  ExampleGame
//

#ifndef PLAYER_UNIT_H
#define PLAYER_UNIT_H

#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
class GridCell;

class PlayerUnit : public BaseUnit {
public:
	PlayerUnit();
	PlayerUnit(Object* object_);
	~PlayerUnit();

	virtual void OnTouch(int touchId, GridCell* touchedCell);
	void OnDeselect();

	static inline ComponentIdType GetTypeId()  { return BaseUnit::GetTypeId(); }
protected:
	void onSelectedTouch();
	void onNavTouch(int touchId, GridCell* touchedCell);

	virtual void onSpecialTouch(int /*touchId*/) {}
	virtual bool isSpecialTouch(int /*touchId*/) {return false;}
	virtual void specialModeActivate() {}
	virtual void calcSpecialTargets() {}

	InputState inputState;
private:
	void init();
	void destroy();

	UnitColorScheme colorScheme;
};

#endif //PLAYER_UNIT_H
