#ifndef TriggerLoot_H
#define TriggerLoot_H

#include "ExampleGame/Components/Triggers/Triggerable.h"

//[[COMPONENT]]//
class TriggerLoot : public Triggerable {
public:
	TriggerLoot();
	TriggerLoot(Object* object_);
	virtual ~TriggerLoot();

	static inline ComponentIdType GetTypeId() { return Triggerable::GetTypeId(); }

	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	virtual void SubscribeToParentSwitch();  // Subscribe to the switch on the parent object, can be used in XML prefabs

protected:
	virtual void onSwitchToggled(bool switchState);

private:
	void init();
	void destroy();
};

#endif // TriggerLoot_H