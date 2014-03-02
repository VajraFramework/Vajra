//
//  TriggerMultiplex.h
//  Created by Matt Kaufmann on 03/02/14.
//

#ifndef TRIGGERMULTIPLEX_H
#define TRIGGERMULTIPLEX_H

#include "ExampleGame/Components/Triggers/Triggerable.h"

// Combine this component with a MultiplexSwitch

//[[COMPONENT]]//
class TriggerMultiplex : public Triggerable {
public:
	TriggerMultiplex();
	TriggerMultiplex(Object* object_);
	virtual ~TriggerMultiplex();

	static inline ComponentIdType GetTypeId() { return Triggerable::GetTypeId(); }

	//[[PROPERTY]]//
	virtual void SetTriggerType(std::string typeStr);
	//[[PROPERTY]]//
	virtual void SetToggleState(bool toggle);
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
};

#endif // TRIGGERMULTIPLEX_H
