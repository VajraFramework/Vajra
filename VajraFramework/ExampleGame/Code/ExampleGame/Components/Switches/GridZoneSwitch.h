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
	virtual void SetRequiredOccupants(unsigned int num);
	//[[PROPERTY]]//
	virtual void SetActivateWhenUnitsInZone(bool b);
	//[[PROPERTY]]//
	void SetDecalType(std::string decalType);

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

protected:
	// @Override
	virtual bool isConditionMet();

	virtual bool doesObjectCountAsOccupant(ObjectIdType id);  // This method can be used to filter out specific objects

	virtual void onUnitEnteredZone(ObjectIdType id);
	virtual void onUnitExitedZone(ObjectIdType id);
private:
	void init();
	void destroy();


	unsigned int requiredOccupants;
	bool activateWhenUnitsInZone;
	std::list<ObjectIdType> occupants;
};

#endif // GRIDZONESWITCH_H
