//
//  TriggerGridZoneActivate.h
//  Created by Matt Kaufmann on 04/24/14.
//

#ifndef TRIGGERGRIDZONEACTIVATE_H
#define TRIGGERGRIDZONEACTIVATE_H

#include "ExampleGame/Components/Triggers/Triggerable.h"
#include "Libraries/glm/glm.hpp"

//[[COMPONENT]]//
class TriggerGridZoneActivate : public Triggerable {
public:
	TriggerGridZoneActivate();
	TriggerGridZoneActivate(Object* object_);
	virtual ~TriggerGridZoneActivate();

	static inline ComponentIdType GetTypeId() { return Triggerable::GetTypeId(); }

	//[[PROPERTY]]//
	virtual void SetTriggerType(std::string typeStr);
	//[[PROPERTY]]//
	virtual void SubscribeToMySwitch();      // Subscribe to the switch on the same object, can be used in XML prefabs
	//[[PROPERTY]]//
	virtual void SubscribeToParentSwitch();  // Subscribe to the switch on the parent object, can be used in XML prefabs

protected:
	virtual void onSwitchToggled(bool switchState);

private:
	void init();
	void destroy();
};

#endif // TRIGGERGRIDZONEACTIVATE_H
