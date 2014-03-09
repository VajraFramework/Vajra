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

void UnitInGridZoneSwitch::SetActivateWhenUnitsInZone(bool b) {
	GridZoneSwitch::SetActivateWhenUnitsInZone(b);
}

void UnitInGridZoneSwitch::SetRequiredUnitType(std::string typeStr) {
	this->SetRequiredUnitType(ConvertStringToUnitType(typeStr));
}

void UnitInGridZoneSwitch::SetRequiredUnitType(UnitType uType)  {
	unsigned long long unitBit = 1;
	unitBit = unitBit << (int)uType;
	this->unitTypeBitMask = unitBit;
}

void UnitInGridZoneSwitch::AddAllowedUnitType(std::string typeStr) {
	this->AddAllowedUnitType(ConvertStringToUnitType(typeStr));
}

void UnitInGridZoneSwitch::AddAllowedUnitType(UnitType uType) {
	unsigned long long unitBit = 1;
	unitBit = unitBit << (int)uType;
	this->unitTypeBitMask = this->unitTypeBitMask | unitBit;
}

void UnitInGridZoneSwitch::AllowAllUnitTypesUpTo(std::string typeStr) {
	this->AllowAllUnitTypesUpTo(ConvertStringToUnitType(typeStr));
}

void UnitInGridZoneSwitch::AllowAllUnitTypesUpTo(UnitType uType) {
	unsigned long long unitBits = 1;
	unitBits = unitBits << ((int)uType + 1);
	this->unitTypeBitMask = unitBits - 1;
}

void UnitInGridZoneSwitch::HandleMessage(MessageChunk messageChunk) {
	GridZoneSwitch::HandleMessage(messageChunk);
}

bool UnitInGridZoneSwitch::doesObjectCountAsOccupant(ObjectIdType id) {
	GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);

	if (gObj != nullptr) {
		BaseUnit* unit = gObj->GetComponent<BaseUnit>();
		if (unit != nullptr) {
			unsigned long long unitBit = 1;
			unitBit = unitBit << (int)unit->GetUnitType();
			return ((unitBit & this->unitTypeBitMask) != 0);
		}
	}
	return false;
}

void UnitInGridZoneSwitch::SetDecalType(std::string decalType) {
	this->setDecalType(decalType);
}

void UnitInGridZoneSwitch::init() {
	this->unitTypeBitMask  = 0;
}

void UnitInGridZoneSwitch::destroy() {
}
