//
//  UnitInGridZoneSwitch.h
//  Created by Matt Kaufmann on 02/20/14.
//

#ifndef UNITINGRIDZONESWITCH_H
#define UNITINGRIDZONESWITCH_H

#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "ExampleGame/Components/Switches/GridZoneSwitch.h"

//[[COMPONENT]]//
class UnitInGridZoneSwitch : public GridZoneSwitch {
public:
	UnitInGridZoneSwitch();
	UnitInGridZoneSwitch(Object* object_);
	virtual ~UnitInGridZoneSwitch();

	static inline ComponentIdType GetTypeId() { return GridZoneSwitch::GetTypeId(); }

	//[[PROPERTY]]//
	virtual void SetSwitchType(std::string typeStr);
	//[[PROPERTY]]//
	virtual void SetResetTime(float t);
	//[[PROPERTY]]//
	virtual void SetRequiredOccupants(unsigned int num);
	//[[PROPERTY]]//
	virtual void SetActivateWhenUnitsInZone(bool b);
	//[[PROPERTY]]//
	void SetRequiredUnitType(std::string typeStr);    // Only allows the specified unit type
	void SetRequiredUnitType(UnitType uType);
	//[[PROPERTY]]//
	void AddAllowedUnitType(std::string typeStr);     // Also allow the specified unit type
	void AddAllowedUnitType(UnitType uType);
	//[[PROPERTY]]//
	void AllowAllUnitTypesUpTo(std::string typeStr);  // Set all units up to the specified type as allowed
	void AllowAllUnitTypesUpTo(UnitType uType);
	//[[PROPERTY]]//
	void SetDecalType(std::string decalType);

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

protected:
	// @Override
	virtual bool doesObjectCountAsOccupant(ObjectIdType id);  // This method can be used to filter out specific objects

private:
	void init();
	void destroy();

	unsigned long long unitTypeBitMask;
};

#endif // UNITINGRIDZONESWITCH_H
