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

	virtual void cancelSpecial();

protected:
	// @Override 
	virtual void amendTouchIndicatorPaths(std::vector<std::string>& pathsToTextures);

	virtual bool isSpecialTouch(int /* touchId */);
	virtual void onSpecialTouch(int /* touchId */);

	virtual void trySpecial(int /* touchId */ );
	virtual void startSpecial();
	virtual void onSpecialEnd();
	virtual void aimSpecial(int /* touchId */ );
private:
	void init();
	void destroy();
	
	glm::vec2 swipeDirectionScreen; // direction of the swipe on the device screen

	GameObject* arrowHead;
	GameObject* arrowTail;
	GridCell* targetedCell;
	glm::vec3 targetLoc;

	glm::vec3 lastPosition;
	GridCell* lastHitCell;
	GridCell* lastCheckedCell;
	void specialUpdate();
	void sendAttackMessage(int gridX, int gridZ, int elevation);
	void checkFinalAttack();

	friend void assassinTweenCallback(ObjectIdType /* gameObjectId */, std::string /* tweenClipName */);
	friend void assassinNumberTweenCallback(float /* fromNumber */, float /* toNumber */, float /*currentNumber*/, std::string /*tweenClipName*/, MessageData1S1I1F* userParams);
	
};

#endif //ASSASSIN_UNIT_H
