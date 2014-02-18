//
//  TriggerTerrainBlock.h
//  Created by Matt Kaufmann on 02/16/14.
//

#ifndef TRIGGERTERRAINBLOCK_H
#define TRIGGERTERRAINBLOCK_H

#include "ExampleGame/Components/Triggers/Triggerable.h"

//[[COMPONENT]]//
class TriggerTerrainBlock : public Triggerable {
public:
	TriggerTerrainBlock();
	TriggerTerrainBlock(Object* object_);
	virtual ~TriggerTerrainBlock();

	static inline ComponentIdType GetTypeId() { return Triggerable::GetTypeId(); }

	//[[PROPERTY]]//
	virtual void SetTriggerType(std::string typeStr);

	//[[PROPERTY]]//
	inline void SetChangeWalkability(bool changeWalkable);
	//[[PROPERTY]]//
	inline void SetChangeVisibility(bool changeVisible);

	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	virtual void SubscribeToMySwitch();      // Subscribe to the switch on the same object, can be used in XML prefabs
	//[[PROPERTY]]//
	virtual void SubscribeToParentSwitch();  // Subscribe to the switch on the parent object, can be used in XML prefabs

protected:
	virtual void onSwitchToggled(bool /*switchState*/);
	virtual void onSwitchActivated();
	virtual void onSwitchDeactivated();

private:
	void init();
	void destroy();

	void startTerrainChange();

	bool changeWalkability;
	bool changeVisibility;
};

void TriggerTerrainBlock::SetChangeWalkability(bool changeWalkable)  { this->changeWalkability = changeWalkable; }
void TriggerTerrainBlock::SetChangeVisibility(bool changeVisible)    { this->changeVisibility  = changeVisible;  }

#endif // TRIGGERTERRAINBLOCK_H
