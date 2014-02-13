//
//  BaseSwitch.h
//  Created by Matt Kaufmann on 02/11/14.
//

#ifndef BASESWITCH_H
#define BASESWITCH_H

#include "Vajra/Common/Components/Component.h"

#include <list>
#include <string>

enum SwitchType {
	SWITCH_TYPE_ONCE,                // Once turned on, stays on permanently
	SWITCH_TYPE_TOGGLE,              // Changes state each time switch condition is met
	SWITCH_TYPE_CONTINUOUS,          // Remains on for as long as condition is met
	SWITCH_TYPE_RESET_ON_ACTIVATE,   // Remains on for a set period of time, then turns off
	SWITCH_TYPE_RESET_ON_DEACTIVATE, // Once turned on, turns off again after a period of time with the condition not being met
	SWITCH_TYPE_INVALID,
};

SwitchType ConvertStringToSwitchType(std::string str);

class BaseSwitch : public Component {
public:
	BaseSwitch();
	BaseSwitch(Object* object_);
	virtual ~BaseSwitch();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	virtual void SetSwitchType(std::string typeStr);
	inline void SetSwitchType(SwitchType st) { this->type = st; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	void AddSubscriber(ObjectIdType subscriberId);
	void RemoveSubscriber(ObjectIdType subscriberId);

protected:
	void update();

	virtual bool isConditionMet() { return false; }

	SwitchType type;

private:
	void init();
	void destroy();

	void setActiveState(bool state);

	bool isActive;
	bool isLocked;
	float resetTime;    // Time in seconds for the switch to reset
	float resetCounter;

	std::list<ObjectIdType> subscribers; // Objects in this list will receive messages when the switch changes state

	static ComponentIdType componentTypeId;
};

#endif // BASESWITCH_H
