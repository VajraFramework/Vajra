//
//  TriggerMovingBlocker.h
//  Created by Matt Kaufmann on 02/26/14.
//

#ifndef TRIGGERMOVINGBLOCKER_H
#define TRIGGERMOVINGBLOCKER_H

#include "ExampleGame/Components/Triggers/Triggerable.h"

// Forward Declarations:
class GridCell;

//[[COMPONENT]]//
class TriggerMovingBlocker : public Triggerable {
public:
	TriggerMovingBlocker();
	TriggerMovingBlocker(Object* object_);
	virtual ~TriggerMovingBlocker();

	static inline ComponentIdType GetTypeId() { return Triggerable::GetTypeId(); }

	//[[PROPERTY]]//
	virtual void SetTriggerType(std::string typeStr);
	//[[PROPERTY]]//
	virtual void SetActivationAudio(std::string audioStr);
	//[[PROPERTY]]//
	virtual void SetDeactivationAudio(std::string audioStr);
	//[[PROPERTY]]//
	virtual void SetToggleState(bool toggle);
	//[[PROPERTY]]//
	inline void SetChangeOnEnterCell(bool changeOnEnter);
	//[[PROPERTY]]//
	inline void SetChangeWalkability(bool changeWalkable);
	//[[PROPERTY]]//
	inline void SetChangeVisibility(bool changeVisible);
	//[[PROPERTY]]//
	void SetTranslation(float x, float y, float z);
	void SetTranslation(glm::vec3 vec);
	//[[PROPERTY]]//
	inline void SetTransitTime(float seconds);

	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	virtual void SubscribeToMySwitch();      // Subscribe to the switch on the same object, can be used in XML prefabs
	//[[PROPERTY]]//
	virtual void SubscribeToParentSwitch();  // Subscribe to the switch on the parent object, can be used in XML prefabs

	//[[PROPERTY]]//
	void SetDecalType(std::string decalType);

protected:
	virtual void onSwitchToggled(bool switchState);

private:
	void init();
	void destroy();

	void startTransformation(bool transformed);
	void startTranslation(bool transformed);
	void onZoneEnteredCell(int gridX, int gridZ);
	void onZoneExitedCell(int gridX, int gridZ);

	bool changeOnEnterCell; // If false, changes the terrain when exiting a cell
	bool changeWalkability;
	bool changeVisibility;
	bool setCellsPassable;

	glm::vec3 translation;
	float transitTime;
	bool isInTransit;

	friend void movingBlockerTweenCallback(ObjectIdType gameObjectId, std::string /*tweenClipName*/);
};

void TriggerMovingBlocker::SetChangeOnEnterCell(bool changeOnEnter)   { this->changeOnEnterCell = changeOnEnter;  }
void TriggerMovingBlocker::SetChangeWalkability(bool changeWalkable)  { this->changeWalkability = changeWalkable; }
void TriggerMovingBlocker::SetChangeVisibility(bool changeVisible)    { this->changeVisibility  = changeVisible;  }
void TriggerMovingBlocker::SetTransitTime(float seconds)              { this->transitTime       = seconds;        }

#endif // TRIGGERMOVINGBLOCKER_H
