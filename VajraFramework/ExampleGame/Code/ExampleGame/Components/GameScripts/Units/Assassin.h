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

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

protected:
	// @Override 
	virtual bool isSpecialTouch(int /* touchId */);
	virtual void onSpecialTouch(int /* touchId */);

	virtual void trySpecial(int /* touchId */ );
	virtual void startSpecial();
	virtual void onSpecialEnd();
	virtual void cancelSpecial();
	virtual void aimSpecial(int /* touchId */ );

	void onGridCellChanged(ObjectIdType id, int gridX, int gridZ);
private:
	void init();
	void destroy();
	
	glm::vec2 swipeDirectionScreen; // direction of the swipe on the device screen

	GameObject* arrowHead;
	GameObject* arrowTail;
	GridCell* targetedCell;
	glm::vec3 targetLoc;

	friend void assassinTweenCallback(ObjectIdType /* gameObjectId */, std::string /* tweenClipName */);
	
};

#endif //ASSASSIN_UNIT_H
