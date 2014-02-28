//
//  TriggerMovingBlocker.h
//  Created by Matt Kaufmann on 02/26/14.
//

#ifndef TRIGGERMOVINGBLOCKER_H
#define TRIGGERMOVINGBLOCKER_H

#include "ExampleGame/Components/Triggers/Triggerable.h"

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
	virtual void SetToggleState(bool toggle);
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
	virtual void update();
	virtual void onSwitchToggled(bool /*switchState*/);

private:
	void init();
	void destroy();

	void startTransformation(bool transformed);
	void startTranslation(bool transformed);
	//void setCellPassable(GridCell* cell, bool passable);

	glm::vec3 translation;
	float transitTime;
	bool isPassable;
	bool isInTransit;

	GridCell* currentCell;

	friend void movingBlockerTweenCallback(ObjectIdType gameObjectId, std::string /*tweenClipName*/);
};

void TriggerMovingBlocker::SetTransitTime(float seconds)  { this->transitTime = seconds; }

#endif // TRIGGERMOVINGBLOCKER_H
