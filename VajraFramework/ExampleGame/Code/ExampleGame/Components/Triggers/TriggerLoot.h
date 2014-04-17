#ifndef TriggerLoot_H
#define TriggerLoot_H

#include "ExampleGame/Components/Triggers/Triggerable.h"

class MessageData1S1I1F;

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

	//[[PROPERTY]]//
	void SetMoneyValue(int v);

	//[[PROPERTY]]//
	void SetActive(bool active);

	void ForceLootGrab();
protected:
	virtual void onSwitchToggled(bool switchState);

private:
	void init();
	void destroy();

	ObjectIdType particleEffectId;
	
	glm::vec3 startPos;
	int moneyValue;

	bool active;
	void lootTweenEnd();

	friend void lootNumberTweenCallback(float fromNumber, float toNumber, float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams);
};

#endif // TriggerLoot_H
