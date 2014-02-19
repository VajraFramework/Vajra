//
//  TriggerLevelDefeat.h
//  Created by Matt Kaufmann on 02/19/14.
//

#ifndef TRIGGERLEVELDEFEAT_H
#define TRIGGERLEVELDEFEAT_H

#include "ExampleGame/Components/Triggers/Triggerable.h"

class TriggerLevelDefeat : public Triggerable {
public:
	TriggerLevelDefeat();
	TriggerLevelDefeat(Object* object_);
	virtual ~TriggerLevelDefeat();

	static inline ComponentIdType GetTypeId() { return Triggerable::GetTypeId(); }

protected:
	virtual void onSwitchActivated();

private:
	void init();
	void destroy();
};

#endif // TRIGGERLEVELDEFEAT_H
