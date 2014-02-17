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

	virtual void trySpecial(int /* touchId */ );
	virtual void startSpecial();
	virtual void onSpecialEnd();

	virtual void touchedCellChanged(GridCell* prevTouchedCell);
private:
	void init();
	void destroy();
	
	void aimSpecial();
	glm::vec2 swipeDirectionScreen; // direction of the swipe on the device screen

	GridCell* targetedCell;
};

#endif //ASSASSIN_UNIT_H
