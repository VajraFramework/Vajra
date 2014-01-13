//
//  PlayerUnit.h
//  ExampleGame
//

#ifndef PLAYER_UNIT_H
#define PLAYER_UNIT_H

#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"

class PlayerUnit : public BaseUnit {
public:
	PlayerUnit();
	PlayerUnit(Object* object_);
	~PlayerUnit();

	virtual void OnTouch(int touchId);
	static inline ComponentIdType GetTypeId()  { return BaseUnit::GetTypeId(); }
protected:
	virtual void OnSpeicalTouch(int touchId) {};
	virtual void IsSpecialTouch(int touchId) {};
	virtual void SpecialModeActivate() {};
	virtual void CalcSpecialTargest() {};

private:
	void init();
	void destroy();

	UnitColorScheme colorScheme;
};

#endif //PLAYER_UNIT_H
