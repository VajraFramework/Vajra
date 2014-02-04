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

	void OnTouch(int touchId, GridCell* touchedCell);
	void OnDeselect();

	static inline ComponentIdType GetTypeId()  { return BaseUnit::GetTypeId(); }
protected:
	void onSelectedTouch();
	void onNavTouch(int touchId, GridCell* touchedCell);
	inline bool getTouchNearUnit() { return this->touchNearUnit; }

	virtual bool isSpecialTouch(int /* touchId */) = 0; 
	virtual void onSpecialTouch(int /* touchId */) = 0;

	virtual void startSpecial() { performingSpecial = true; }
	virtual void onSpecialEnd() { performingSpecial = false; }

	InputState inputState;

	glm::vec2 touchStartPos;
private:
	void init();
	void destroy();

	GameObject* gameObjectRef;
	UnitColorScheme colorScheme;
	bool touchNearUnit;
	void setTouchNearUnit(); 

	bool performingSpecial;
};

#endif //PLAYER_UNIT_H
