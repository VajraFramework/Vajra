//
//  TriggerElevationChange.h
//  Created by Matt Kaufmann on 02/15/14.
//

#ifndef TRIGGERELEVATIONCHANGE_H
#define TRIGGERELEVATIONCHANGE_H

#include "ExampleGame/Components/Triggers/Triggerable.h"
#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1I1F.h"

#include <list>

//[[COMPONENT]]//
class TriggerElevationChange : public Triggerable {
public:
	TriggerElevationChange();
	TriggerElevationChange(Object* object_);
	virtual ~TriggerElevationChange();

	static inline ComponentIdType GetTypeId() { return Triggerable::GetTypeId(); }

	//[[PROPERTY]]//
	virtual void SetTriggerType(std::string typeStr);
	//[[PROPERTY]]//
	inline void SetElevationDiff(int diff);
	//[[PROPERTY]]//
	inline void SetTransitTime(float seconds);
	//[[PROPERTY]]//
	void SetRaisedState(bool raised);

	virtual void HandleMessage(MessageChunk messageChunk);

#ifdef DEBUG
	//[[PROPERTY]]//
	virtual void SubscribeToSwitchObject();  // Debug-only method, Trigger subscribes to itself
#endif

protected:
	virtual void onSwitchToggled(bool /*switchState*/);
	virtual void onSwitchActivated();
	virtual void onSwitchDeactivated();

	void onUnitEnteredZone(ObjectIdType id);
	void onUnitExitedZone(ObjectIdType id);

private:
	void init();
	void destroy();

	int elevationChange;  // The amount of elevation change when this trigger activates
	float transitTime;
	bool isRaised;
	bool isInTransit;

	std::list<ObjectIdType> unitsInZone;

	friend void elevationChangeTweenCallback(ObjectIdType gameObjectId, std::string tweenClipName);
};

void TriggerElevationChange::SetElevationDiff(int diff)     { this->elevationChange = diff; }
void TriggerElevationChange::SetTransitTime(float seconds)  { this->transitTime = seconds;  }

#endif // TRIGGERELEVATIONCHANGE_H
