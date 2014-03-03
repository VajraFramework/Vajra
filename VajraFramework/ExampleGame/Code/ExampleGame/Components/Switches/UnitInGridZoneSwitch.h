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
	void SetRequiredUnitType(std::string typeStr);
	inline void SetRequiredUnitType(UnitType uType);
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

	UnitType requiredUnitType;
};

void UnitInGridZoneSwitch::SetRequiredUnitType(UnitType uType)  { this->requiredUnitType = uType; }

#endif // UNITINGRIDZONESWITCH_H
