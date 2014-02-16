//
//  GoalZoneSwitch.h
//

#ifndef GOALZONE_SWITCH_H
#define GOALZONE_SWITCH_H

#include "ExampleGame/Components/Switches/GridZoneSwitch.h"

#include <string>

//[[COMPONENT]]//
class GoalZoneSwitch : public GridZoneSwitch {
public:
	GoalZoneSwitch();
	GoalZoneSwitch(Object* object_);
	virtual ~GoalZoneSwitch();

	static inline ComponentIdType GetTypeId() { return BaseSwitch::GetTypeId(); }

protected:
	virtual bool doesObjectCountAsOccupant(ObjectIdType id);  // This method can be used to filter out specific objects

private:
	void init();
	void destroy();
};
#endif // GOALZONE_SWITCH_H
