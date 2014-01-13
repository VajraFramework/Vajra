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
	void OnSelectedTouch();
	void OnNavTouch(int touchId, GridCell* touchedCell);

	virtual void OnSpecialTouch(int touchId) {};
	virtual void IsSpecialTouch(int touchId) {};
	virtual void SpecialModeActivate() {};
	virtual void CalcSpecialTargest() {};

	InputState inputState = InputState::INPUT_STATE_WAIT;
private:
	void init();
	void destroy();

	UnitColorScheme colorScheme;
};

#endif //PLAYER_UNIT_H
