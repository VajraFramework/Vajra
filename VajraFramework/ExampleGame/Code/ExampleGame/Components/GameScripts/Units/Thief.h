//
//  Thief.h
//  ExampleGame
//

#ifndef THIEF_UNIT_H
#define THIEF_UNIT_H

#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"

#include <list>
#include <map>

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

	virtual void touchedCellChanged(GridCell* prevTouchedCell);
private:
	void init();
	void destroy();
	
	void updateLegalTagets();

	void tweenInTargets();
	void tweenOutTargets();

	void createTargets();
	void deleteTargets();

	GridCell* targetedCell;

	std::vector<GridCell*> legalTargets;
	std::map<GridCell* /*cell*/, GameObject*> targetIndicators;
	friend void thiefTweenCallback(ObjectIdType /* gameObjectId */, std::string /* tweenClipName */);
	friend void thiefNumberTweenCallback(float fromNumber, float toNumber, float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams);
	

};

#endif //THIEF_UNIT_H
