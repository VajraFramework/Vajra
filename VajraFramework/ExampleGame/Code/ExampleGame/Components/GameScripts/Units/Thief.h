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

protected:
	// @Override 
	virtual bool isSpecialTouch(int /* touchId */);
	virtual void onSpecialTouch(int /* touchId */);

	virtual void startSpecial();
	virtual void onSpecialEnd();
private:
	void init();
	void destroy();
	
	GridCell* targetedCell;

	friend void thiefTweenCallback(ObjectIdType /* gameObjectId */, std::string /* tweenClipName */);

};

#endif //THIEF_UNIT_H
