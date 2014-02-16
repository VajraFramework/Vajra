//
//  Triggerable.h
//  Created by Matt Kaufmann on 02/11/14.
//

#ifndef TRIGGERABLE_H
#define TRIGGERABLE_H

#include "Vajra/Common/Components/Component.h"

#include <list>
#include <string>

enum TriggerType {
	TRIGGER_TYPE_ALL,      // The trigger will activate only once all switches are on at once
	TRIGGER_TYPE_ANY,      // The trigger will activate if any switches are turned on
	TRIGGER_TYPE_INVALID,
};

TriggerType ConvertStringToTriggerType(std::string str);

//[[COMPONENT]]//
class Triggerable : public Component {
public:
	Triggerable();
	Triggerable(Object* object_);
	virtual ~Triggerable();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	virtual void SetTriggerType(std::string typeStr);
	inline void SetTriggerType(TriggerType st) { this->type = st; }

	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	virtual void SubscribeToMySwitch();  // Subscribe to the switch on the same object, can be used in XML prefabs
	void SubscribeToSwitchObject(ObjectIdType switchId);   // This must be called before this component can trigger
	void UnsubscribeToSwitchObject(ObjectIdType switchId);

protected:
	virtual void onSwitchToggled(bool /*switchState*/) { }
	virtual void onSwitchActivated() { }
	virtual void onSwitchDeactivated() { }

	TriggerType type;

private:
	void init();
	void destroy();

	bool shouldTriggerActivate();
	bool shouldTriggerDeactivate();

	std::list<ObjectIdType> subscriptions;
	int activeSwitches;

	static ComponentIdType componentTypeId;
};

#endif // TRIGGERABLE_H
