//
//  Thief.h
//  ExampleGame
//

#ifndef THIEF_UNIT_H
#define THIEF_UNIT_H

#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
class GridCell;

//[[COMPONENT]]//
class Thief : public PlayerUnit {
public:
	Thief();
	Thief(Object* object_);
	~Thief();

	static inline ComponentIdType GetTypeId()  { return BaseUnit::GetTypeId(); }

	virtual bool isSpecialTouch(int /* touchId */);
	virtual void onSpecialTouch(int /* touchId */);

	virtual void startSpecial();
	virtual void onSpecialEnd();
private:
	void init();
	void destroy();
	
	GridCell* targetedCell;

	const float allowedMovementInPixels = 10.0f;
	const float longPressInSeconds = 0.5f;
};

#endif //THIEF_UNIT_H
