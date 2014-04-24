//
//  BaseSwitch.h
//  Created by Matt Kaufmann on 02/11/14.
//

#ifndef BASESWITCH_H
#define BASESWITCH_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1I1F.h"

#include <list>
#include <string>

enum SwitchType {
	SWITCH_TYPE_ONCE,                // Once turned on, stays on permanently
	SWITCH_TYPE_TOGGLE,              // Changes state each time switch condition is met
	SWITCH_TYPE_CONTINUOUS,          // Remains on for as long as condition is met
	SWITCH_TYPE_WARMUP,              // Condition must remain true for a period of time before the switch will turn on
	SWITCH_TYPE_RESET_ON_ACTIVATE,   // Remains on for a set period of time, then turns off
	SWITCH_TYPE_RESET_ON_DEACTIVATE, // Once turned on, turns off again after a period of time with the condition not being met
	SWITCH_TYPE_INVALID,
};

// Forward Declarations:
class GameObject;

SwitchType ConvertStringToSwitchType(std::string str);

//[[COMPONENT]]//
class BaseSwitch : public Component {
public:
	BaseSwitch();
	BaseSwitch(Object* object_);
	virtual ~BaseSwitch();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	//[[PROPERTY]]//
	virtual void SetSwitchType(std::string typeStr);
	inline void SetSwitchType(SwitchType st) { this->type = st; }

	//[[PROPERTY]]//
	virtual void SetResetTime(float t);
	inline float GetResetTime() { return this->resetTime; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	void AddSubscriber(ObjectIdType subscriberId);
	void RemoveSubscriber(ObjectIdType subscriberId);

	inline bool IsActive()     { return this->isActive;  }
	std::string GetDecalType() { return this->decalType; }

	//[[PROPERTY]]//
	void SetDecalType(std::string decalType);

protected:
	virtual bool isConditionMet() { return false; }

	void setConditionState(bool state);  // This gets called whenever the switch's conditional state changes

	void setDecalType(std::string decalType_);

	virtual void onSwitchCallback(float /* normalizedTime */) {}
	SwitchType type;

	GameObject* gameObjectRef;
private:
	void init();
	void destroy();

	void setActiveState(bool state);     // This should only be called by setConditionalState

	bool isActive;
	float resetTime;    // Time in seconds for the switch to reset

	std::string decalType;
	GameObject* decalRef;

	std::list<ObjectIdType> subscribers; // Objects in this list will receive messages when the switch changes state

	static ComponentIdType componentTypeId;

	friend void baseSwitchNumberTweenCallback(float fromNumber, float toNumber, float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams);
};

#endif // BASESWITCH_H
