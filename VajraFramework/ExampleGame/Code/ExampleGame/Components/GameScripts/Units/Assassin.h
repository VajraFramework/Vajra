//
//  Assassin.h
//  ExampleGame
//

#ifndef ASSASSIN_UNIT_H
#define ASSASSIN_UNIT_H

#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
class GridCell;

//[[COMPONENT]]//
class Assassin : public PlayerUnit {
public:
	Assassin();
	Assassin(Object* object_);
	~Assassin();

	static inline ComponentIdType GetTypeId()  { return BaseUnit::GetTypeId(); }

protected:
	// @Override 
	virtual bool isSpecialTouch(int /* touchId */);
	virtual void onSpecialTouch(int /* touchId */);

	virtual void startSpecial();
	virtual void onSpecialEnd();
private:
	void init();
	void destroy();
	
	glm::vec2 swipeDirectionScreen; // direction of the swipe on the device screen

	GridCell* targetedCell;

	const float attackSpeed = 10.0f;
	const float swipeDistanceInPixels = 50.0f;
	const float maxSwipeLengthInSeconds = 1.0f;
};

#endif //ASSASSIN_UNIT_H
