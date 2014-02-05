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

	float getMoveSpeed() { return this->moveSpeed; }
private:
	void init();
	void destroy();

	UnitColorScheme colorScheme;
	bool touchNearUnit;
	void setTouchNearUnit(); 

	bool performingSpecial;

	// default values for units
	float moveSpeed;
	float turnSpeedDegrees;
	const float nearTouchDist = 1.5f;
};

#endif //PLAYER_UNIT_H
