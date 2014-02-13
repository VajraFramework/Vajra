//
//  GridZoneSwitch.h
//  Created by Matt Kaufmann on 02/12/14.
//

#ifndef GRIDZONESWITCH_H
#define GRIDZONESWITCH_H

#include "ExampleGame/Components/Switches/BaseSwitch.h"

#include <string>

//[[COMPONENT]]//
class GridZoneSwitch : public BaseSwitch {
public:
	GridZoneSwitch();
	GridZoneSwitch(Object* object_);
	virtual ~GridZoneSwitch();

	static inline ComponentIdType GetTypeId() { return BaseSwitch::GetTypeId(); }

	//[[PROPERTY]]//
	virtual void SetSwitchType(std::string typeStr);
	//[[PROPERTY]]//
	virtual void SetResetTime(float t);
	//[[PROPERTY]]//
	inline void SetRequiredOccupants(unsigned int num);

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

protected:
	// @Override
	virtual bool isConditionMet();

	virtual bool countObjectAsOccupant(ObjectIdType id);  // This method can be used to filter out specific objects

private:
	void init();
	void destroy();

	void onUnitEnteredZone(ObjectIdType id);
	void onUnitExitedZone(ObjectIdType id);

	unsigned int requiredOccupants;
	std::list<ObjectIdType> occupants;
};

void GridZoneSwitch::SetRequiredOccupants(unsigned int num) { this->requiredOccupants = num; }

#endif // GRIDZONESWITCH_H
