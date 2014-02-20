//
//  TriggerLevelVictory.h
//  Created by Matt Kaufmann on 02/19/14.
//

#ifndef TRIGGERLEVELVICTORY_H
#define TRIGGERLEVELVICTORY_H

#include "ExampleGame/Components/Triggers/Triggerable.h"

class TriggerLevelVictory : public Triggerable {
public:
	TriggerLevelVictory();
	TriggerLevelVictory(Object* object_);
	virtual ~TriggerLevelVictory();

	static inline ComponentIdType GetTypeId() { return Triggerable::GetTypeId(); }

protected:
	virtual void onSwitchActivated();

private:
	void init();
	void destroy();
};

#endif // TRIGGERLEVELVICTORY_H
