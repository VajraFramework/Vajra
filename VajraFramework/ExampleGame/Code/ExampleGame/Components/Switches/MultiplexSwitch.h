//
//  MultiplexSwitch.h
//  Created by Matt Kaufmann on 02/16/14.
//

#ifndef MULTIPLEXSWITCH_H
#define MULTIPLEXSWITCH_H

#include "ExampleGame/Components/Switches/BaseSwitch.h"
#include "ExampleGame/Components/Triggers/Triggerable.h"

// Combine this component with a TriggerMultiplex

//[[COMPONENT]]//
class MultiplexSwitch : public BaseSwitch {
public:
	MultiplexSwitch();
	MultiplexSwitch(Object* object_);
	virtual ~MultiplexSwitch();

	static inline ComponentIdType GetTypeId() { return BaseSwitch::GetTypeId(); }

	//[[PROPERTY]]//
	virtual void SetSwitchType(std::string typeStr);

	//[[PROPERTY]]//
	virtual void SetResetTime(float t);

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

private:
	void init();
	void destroy();
};

#endif // MULTIPLEXSWITCH_H
