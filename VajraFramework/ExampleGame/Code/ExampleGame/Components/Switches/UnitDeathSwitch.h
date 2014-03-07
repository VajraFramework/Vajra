//
//  UnitDeathSwitch.h
//  Created by Matt Kaufmann on 03/06/14.
//

#ifndef UNITDEATHSWITCH_H
#define UNITDEATHSWITCH_H

#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "ExampleGame/Components/Switches/BaseSwitch.h"

//[[COMPONENT]]//
class UnitDeathSwitch : public BaseSwitch {
public:
	UnitDeathSwitch();
	UnitDeathSwitch(Object* object_);
	virtual ~UnitDeathSwitch();

	static inline ComponentIdType GetTypeId() { return BaseSwitch::GetTypeId(); }

	//[[PROPERTY]]//
	virtual void SetSwitchType(std::string typeStr);
	//[[PROPERTY]]//
	virtual void SetResetTime(float t);
	//[[PROPERTY]]//
	inline void SetRequiredDeaths(unsigned int numDeaths);
	//[[PROPERTY]]//
	void SetRequiredUnitType(std::string typeStr);    // Only allows the specified unit type
	void SetRequiredUnitType(UnitType uType);
	//[[PROPERTY]]//
	void AddAllowedUnitType(std::string typeStr);     // Also allow the specified unit type
	void AddAllowedUnitType(UnitType uType);
	//[[PROPERTY]]//
	void AllowAllUnitTypesUpTo(std::string typeStr);  // Set all units up to the specified type as allowed
	void AllowAllUnitTypesUpTo(UnitType uType);

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

protected:
	// @Override
	//virtual bool isConditionMet();

	void onUnitWasKilled(ObjectIdType id);
	bool doesUnitDeathCount(ObjectIdType id);

private:
	void init();
	void destroy();

	unsigned int requiredDeaths;
	unsigned long long unitTypeBitMask;
};

void UnitDeathSwitch::SetRequiredDeaths(unsigned int numDeaths)  { this->requiredDeaths = numDeaths; }

#endif // UNITDEATHSWITCH_H
