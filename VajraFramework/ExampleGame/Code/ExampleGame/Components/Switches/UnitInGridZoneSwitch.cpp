//
//  UnitInGridZoneSwitch.cpp
//  Created by Matt Kaufmann on 02/20/14.
//

#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Components/Switches/UnitInGridZoneSwitch.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"

UnitInGridZoneSwitch::UnitInGridZoneSwitch() : GridZoneSwitch() {
	this->init();
}

UnitInGridZoneSwitch::UnitInGridZoneSwitch(Object* object_) : GridZoneSwitch(object_) {
	this->init();
}

UnitInGridZoneSwitch::~UnitInGridZoneSwitch() {
	this->destroy();
}

void UnitInGridZoneSwitch::SetSwitchType(std::string typeStr) {
	GridZoneSwitch::SetSwitchType(typeStr);
}

void UnitInGridZoneSwitch::SetResetTime(float t) {
	GridZoneSwitch::SetResetTime(t);
}

void UnitInGridZoneSwitch::SetRequiredOccupants(unsigned int num) {
	GridZoneSwitch::SetRequiredOccupants(num);
}

void UnitInGridZoneSwitch::SetRequiredUnitType(std::string typeStr) {
	this->SetRequiredUnitType(ConvertStringToUnitType(typeStr));
}

void UnitInGridZoneSwitch::HandleMessage(MessageChunk messageChunk) {
	GridZoneSwitch::HandleMessage(messageChunk);
}

bool UnitInGridZoneSwitch::doesObjectCountAsOccupant(ObjectIdType id) {
	GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);

	if (gObj != nullptr) {
		BaseUnit* unit = gObj->GetComponent<BaseUnit>();

		if (unit != nullptr) {
			return (unit->GetUnitType() == this->requiredUnitType);
		}
	}
	return false;
}

void UnitInGridZoneSwitch::init() {
	this->requiredUnitType = UNIT_TYPE_UNKNOWN;
}

void UnitInGridZoneSwitch::destroy() {

}
