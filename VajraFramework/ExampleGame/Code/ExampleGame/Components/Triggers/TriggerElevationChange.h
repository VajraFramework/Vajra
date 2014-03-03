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
	inline void SetRaisedState(bool raised);  // Only call this function from XML to set the initial state.

	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	virtual void SubscribeToMySwitch();      // Subscribe to the switch on the same object, can be used in XML prefabs
	//[[PROPERTY]]//
	virtual void SubscribeToParentSwitch();  // Subscribe to the switch on the parent object, can be used in XML prefabs

protected:
	virtual void onSwitchToggled(bool switchState);

	void onUnitEnteredZone(ObjectIdType id);
	void onUnitExitedZone(ObjectIdType id);

private:
	void init();
	void destroy();

	void startTransition(bool raised);
	void startPositionTween(bool raised);
	void setCellsInGridZonePassable(bool pass);
	void changeCellElevations(bool raised);

	int elevationChange;  // The amount of elevation change when this trigger activates
	float transitTime;

	std::list<ObjectIdType> unitsInZone;

	GameObject* gameObjectRef;

	friend void elevationChangeTweenCallback(ObjectIdType gameObjectId, std::string tweenClipName);
};

void TriggerElevationChange::SetRaisedState(bool raised)    { this->SetToggleState(raised); }
void TriggerElevationChange::SetElevationDiff(int diff)     { this->elevationChange = diff; }
void TriggerElevationChange::SetTransitTime(float seconds)  { this->transitTime = seconds;  }

#endif // TRIGGERELEVATIONCHANGE_H
