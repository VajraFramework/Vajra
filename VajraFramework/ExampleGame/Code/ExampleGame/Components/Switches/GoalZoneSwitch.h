#ifndef GOALZONESWITCH_H
#define GOALZONESWITCH_H

#include "ExampleGame/Components/Switches/BaseSwitch.h"
#include "ExampleGame/Components/Switches/UnitInGridZoneSwitch.h"

#include <string>

class SpriteRenderer;

//[[COMPONENT]]//
class GoalZoneSwitch : public UnitInGridZoneSwitch {
public:
	GoalZoneSwitch();
	GoalZoneSwitch(Object* object_);
	virtual ~GoalZoneSwitch();

	static inline ComponentIdType GetTypeId() { return UnitInGridZoneSwitch::GetTypeId(); }

	//[[PROPERTY]]//
	virtual void SetRequiredUnitType(std::string typeStr);    // Only allows the specified unit type

	virtual void onUnitEnteredZone(ObjectIdType id);
	virtual void onUnitExitedZone(ObjectIdType id);

protected:
	virtual void onSwitchCallback(float normalizedTime);
private:
	void init();
	void destroy();

	SpriteRenderer* spriteRendererRef;
	glm::vec4 zoneColor;

	friend void goalZoneSwitchNumberTweenCallback(float fromNumber, float toNumber, float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams);
};

#endif // GOALZONESWITCH_H
