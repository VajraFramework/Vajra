//
//  PlayerUnit.h
//  ExampleGame
//

#ifndef PLAYER_UNIT_H
#define PLAYER_UNIT_H

#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"

class GridCell;
class MessageData1S1I1F;

class PlayerUnit : public BaseUnit {

#define MOVE_SPEED 2.5f
#define TURN_SPEED_DEG 1000.0f
#define NEAR_TOUCH_DIST 1.5f 
	
#define GOOD_TOUCH 0
#define BAD_TOUCH  1
#define ASSASSIN_SPECIAL 2
#define THIEF_SPECIAL 3

public:
	PlayerUnit();
	PlayerUnit(Object* object_);
	~PlayerUnit();

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	void OnTouch(int touchId, GridCell* touchedCell);
	void OnDeselect();

	void OnTransitionZoneEntered(GridCell* newTarget);

	static inline ComponentIdType GetTypeId()  { return BaseUnit::GetTypeId(); }
protected:
	void onSelectedTouch();
	void onNavTouch(int touchId, GridCell* touchedCell);
	inline bool getTouchNearUnit() { return this->touchNearUnit; }

	virtual bool isSpecialTouch(int /* touchId */) = 0; 
	virtual void onSpecialTouch(int /* touchId */) = 0;

	virtual void trySpecial(int /* touchId */) = 0;
	virtual void startSpecial();
	virtual void onSpecialEnd();
	virtual void cancelSpecial();
	virtual void touchedCellChanged(GridCell* prevTouchedCell);

	InputState inputState;

	glm::vec2 touchStartPos;
	glm::vec3 specialStartPos;

	void startTouchIndicatorPulse();
	void SetTouchIndicatorSprite(int /*index*/ );
	void SetTouchIndicatorCell(GridCell*);
	void TouchIndicatorLookAt(GridCell* /*target*/);
	void GridPlaneLookAt(GameObject* /*plane*/, GridCell* /*target*/);
	void SetTouchIndicatorVisible(bool /*visibilty*/);

	

	inline GridCell* GetCurrentTouchedCell() { return this->currentTouchedCell; }
private:
	void init();
	void destroy();

	GameObject* touchIndicatorRef;
	GridCell* currentTouchedCell;

	UnitColorScheme colorScheme;
	
	bool touchNearUnit;
	void setTouchNearUnit(); 

	bool unitHasTouchFocus;

	friend void playerUnitNumberTweenCallback(float fromNumber, float toNumber, float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams);
	friend class LevelLoader;
};

#endif //PLAYER_UNIT_H
