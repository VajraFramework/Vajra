//
//  PlayerUnit.h
//  ExampleGame
//

#ifndef PLAYER_UNIT_H
#define PLAYER_UNIT_H

#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"

class GridCell;
class GridNavigator;
class MessageData1S1I1F;

class PlayerUnit : public BaseUnit {

#define MOVE_SPEED 2.75f
#define TURN_SPEED_DEG 1000.0f
#define NEAR_TOUCH_DIST 1.5f 
	
#define GOOD_TOUCH 0
#define BAD_TOUCH  1
#define PLAYER_NUM_TOUCH_IMAGES 2

public:
	PlayerUnit();
	PlayerUnit(Object* object_);
	~PlayerUnit();

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	void OnTouch(int touchId, GridCell* touchedCell);
	void OnDeselect();

	// @Override
	virtual void OnTransitionZoneEntered(GridCell* newTarget);
	virtual bool CanBeKilledBy(ObjectIdType id, glm::vec3 source);
	static inline ComponentIdType GetTypeId()  { return BaseUnit::GetTypeId(); }
	virtual void cancelSpecial();
protected:
	GameObject* touchIndicatorRef;

	virtual void Kill();

	void onSelectedTouch();
	void onNavTouch(int touchId, GridCell* touchedCell);

	void createTouchIndicator();
	virtual void amendTouchIndicatorPaths(std::vector<std::string>& pathsToTextures) = 0;
	inline bool getTouchNearUnit() { return this->touchNearUnit; }

	virtual bool isSpecialTouch(int /* touchId */) = 0; 
	virtual void onSpecialTouch(int /* touchId */) = 0;

	virtual void trySpecial(int /* touchId */) = 0;
	virtual void startSpecial();
	virtual void onSpecialEnd();
	virtual void touchedCellChanged(GridCell* prevTouchedCell);
	virtual void aimSpecial(int touchId) = 0;
	InputState inputState;

	glm::vec2 touchStartPos;
	glm::vec3 specialStartPos;

	void startTouchIndicatorPulse();
	void SetTouchIndicatorSprite(int /*index*/ );

	void GridPlaneSetPos(GameObject* /*plane*/, GridCell* /*target*/);
	void GridPlaneSetPos(GameObject* /*plane*/, GridCell* /*target*/, int elevation);
	void GridPlaneSetPos(GameObject* /*plane*/, glm::vec3 /*target*/);

	void GridPlaneLookAt(GameObject* /*plane*/, GridCell* /*target*/);
	void GridPlaneLookAt(GameObject* /*plane*/, glm::vec3 /*target*/);

	void SetTouchIndicatorVisible(bool /*visibilty*/);	

	glm::vec3 GetOffsetFromCell(GridCell* /*targetCell*/, float additionalOffset = 0);

	inline GridCell* GetCurrentTouchedCell() { return this->currentTouchedCell; }
	
private:
	void init();
	void destroy();

	GridCell* currentTouchedCell;

	UnitColorScheme colorScheme;
	
	bool touchNearUnit;
	void setTouchNearUnit(); 

	bool unitHasTouchFocus;

	friend void playerUnitNumberTweenCallback(float fromNumber, float toNumber, float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams);
	friend class LevelLoader;
};

#endif //PLAYER_UNIT_H
