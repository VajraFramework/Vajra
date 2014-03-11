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

	virtual void cancelSpecial();
protected:
	// @Override 
	virtual void amendTouchIndicatorPaths(std::vector<std::string>& pathsToTextures);

	virtual bool isSpecialTouch(int /* touchId */);
	virtual void onSpecialTouch(int /* touchId */);

	virtual void trySpecial(int /* touchId */) {}
	virtual void startSpecial();
	virtual void onSpecialEnd();

	virtual void aimSpecial(int /* touchId */);

	virtual void start();
private:
	void init();
	void destroy();
	
	void updateLegalTagets();

	void tweenInTargets();
	void tweenOutTargets();

	void createTargets();
	void updateTargets();
	void deleteTargets();
	void setIndicators();

	GridCell* targetedCell;

	int textureIndexForElevation(int cellElevation);

	void scaleUpIndicator(GridCell* /*cell*/);
	void scaleDownIndicator(GridCell* /*cell*/);
	GridCell* getNearCellTowardsUnit(int touchId);

	bool tweenInFinished;
	std::list<GridCell*> cellsInRange;
	std::vector<GridCell*> legalTargets;
	std::map<GridCell* /*cell*/, GameObject*> activeTargetIndicators;
	std::vector<GameObject*> targetIndicators;

	// poof effects
	void createPoofEffects();
	void beginPoof(ObjectIdType /*poofId*/);
	ObjectIdType startPoofId;
	ObjectIdType endPoofId;

	friend void thiefTweenCallback(ObjectIdType /* gameObjectId */, std::string /* tweenClipName */);
	friend void thiefNumberTweenCallback(float fromNumber, float toNumber, float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams);
	

};

#endif //THIEF_UNIT_H
