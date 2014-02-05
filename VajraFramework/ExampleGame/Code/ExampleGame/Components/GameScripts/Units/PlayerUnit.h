//
//  PlayerUnit.h
//  ExampleGame
//

#ifndef PLAYER_UNIT_H
#define PLAYER_UNIT_H

#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"

class GameObject;
class GridCell;

class PlayerUnit : public BaseUnit {

// default values for player units
#define MOVE_SPEED 2.5f
#define TURN_SPEED_DEG 360.0f
#define NEAR_TOUCH_DIST 1.5f 

public:
	PlayerUnit();
	PlayerUnit(Object* object_);
	~PlayerUnit();

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	void OnTouch(int touchId, GridCell* touchedCell);
	void OnDeselect();

	inline bool GetPerformingSpecial() { return this->performingSpecial; }
	static inline ComponentIdType GetTypeId()  { return BaseUnit::GetTypeId(); }
protected:
	void onSelectedTouch();
	void onNavTouch(int touchId, GridCell* touchedCell);
	inline bool getTouchNearUnit() { return this->touchNearUnit; }

	virtual bool isSpecialTouch(int /* touchId */) = 0; 
	virtual void onSpecialTouch(int /* touchId */) = 0;

	virtual void startSpecial();
	virtual void onSpecialEnd();

	InputState inputState;

	glm::vec2 touchStartPos;

private:
	void init();
	void destroy();

	GameObject* touchIndicator;
	GridCell* currentTouchedCell;

	void touchedCellChanged();

	UnitColorScheme colorScheme;
	
	bool touchNearUnit;
	void setTouchNearUnit(); 

	bool performingSpecial;


	friend class LevelLoader;
};

#endif //PLAYER_UNIT_H
