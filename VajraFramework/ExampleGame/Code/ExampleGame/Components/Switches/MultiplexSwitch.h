//
//  MultiplexSwitch.h
//  Created by Matt Kaufmann on 02/16/14.
//

#ifndef MULTIPLEXSWITCH_H
#define MULTIPLEXSWITCH_H

#include "ExampleGame/Components/Switches/BaseSwitch.h"
#include "ExampleGame/Components/Triggers/Triggerable.h"

//[[COMPONENT]]//
class MultiplexSwitch : public BaseSwitch {
public:
	MultiplexSwitch();
	MultiplexSwitch(Object* object_);
	virtual ~MultiplexSwitch();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	//[[PROPERTY]]//
	virtual void SetSwitchType(std::string typeStr);

	//[[PROPERTY]]//
	virtual void SetResetTime(float t);

	//[[PROPERTY]]//
	void SetTriggerType(std::string typeStr);
	inline void SetTriggerType(TriggerType st) { this->triggerType = st; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	void SubscribeToParentSwitch();  // Subscribe to the switch on the parent object, can be used in XML prefabs
	void SubscribeToSwitchObject(ObjectIdType switchId);   // This must be called before this component can trigger
	void UnsubscribeToSwitchObject(ObjectIdType switchId);

protected:
	// These methods trigger in response to the switches the multiplex subscribes to.
	void onSwitchToggled(bool /*switchState*/);
	void onSwitchActivated();
	void onSwitchDeactivated();

	virtual bool isConditionMet() { return false; }

	TriggerType triggerType;

private:
	void init();
	void destroy();

	bool shouldTriggerActivate();
	bool shouldTriggerDeactivate();

	std::list<ObjectIdType> subscriptions;
	int activeSwitches;

	static ComponentIdType componentTypeId;
};

#endif // MULTIPLEXSWITCH_H
