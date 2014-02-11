//
//  Thief.h
//  ExampleGame
//

#ifndef THIEF_UNIT_H
#define THIEF_UNIT_H

#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"

#include <list>

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

	virtual void trySpecial(int /* touchId */) {}
	virtual void startSpecial();
	virtual void onSpecialEnd();

	virtual void touchedCellChanged();
private:
	void init();
	void destroy();
	
	void updateLegalTagets();

	GridCell* targetedCell;

	std::vector<GridCell*> legalTargets;
	friend void thiefTweenCallback(ObjectIdType /* gameObjectId */, std::string /* tweenClipName */);

};

#endif //THIEF_UNIT_H
