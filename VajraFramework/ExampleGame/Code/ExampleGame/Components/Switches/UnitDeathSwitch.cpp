//
//  UnitDeathSwitch.cpp
//  Created by Matt Kaufmann on 03/06/14.
//

#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Components/Switches/UnitDeathSwitch.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"

UnitDeathSwitch::UnitDeathSwitch() : BaseSwitch() {
	this->init();
}

UnitDeathSwitch::UnitDeathSwitch(Object* object_) : BaseSwitch(object_) {
	this->init();
}

UnitDeathSwitch::~UnitDeathSwitch() {
	this->destroy();
}

void UnitDeathSwitch::init() {
	this->requiredDeaths = 1;
	this->unitTypeBitMask = 0;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_UNIT_KILLED, this->GetTypeId(), false);
}

void UnitDeathSwitch::destroy() {

}

void UnitDeathSwitch::SetSwitchType(std::string typeStr) {
	BaseSwitch::SetSwitchType(typeStr);
}

void UnitDeathSwitch::SetResetTime(float t) {
	BaseSwitch::SetResetTime(t);
}

void UnitDeathSwitch::SetRequiredUnitType(std::string typeStr) {
	this->SetRequiredUnitType(ConvertStringToUnitType(typeStr));
}

void UnitDeathSwitch::SetRequiredUnitType(UnitType uType)  {
	unsigned long long unitBit = 1;
	unitBit = unitBit << (int)uType;
	this->unitTypeBitMask = unitBit;
}

void UnitDeathSwitch::AddAllowedUnitType(std::string typeStr) {
	this->AddAllowedUnitType(ConvertStringToUnitType(typeStr));
}

void UnitDeathSwitch::AddAllowedUnitType(UnitType uType) {
	unsigned long long unitBit = 1;
	unitBit = unitBit << (int)uType;
	this->unitTypeBitMask = this->unitTypeBitMask | unitBit;
}

void UnitDeathSwitch::AllowAllUnitTypesUpTo(std::string typeStr) {
	this->AllowAllUnitTypesUpTo(ConvertStringToUnitType(typeStr));
}

void UnitDeathSwitch::AllowAllUnitTypesUpTo(UnitType uType) {
	unsigned long long unitBits = 1;
	unitBits = unitBits << ((int)uType + 1);
	this->unitTypeBitMask = unitBits - 1;
}

void UnitDeathSwitch::HandleMessage(MessageChunk messageChunk) {
	BaseSwitch::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_UNIT_KILLED:
			this->onUnitWasKilled(messageChunk->GetSenderId());
			break;
	}
}
/*
bool UnitDeathSwitch::isConditionMet() {

}
*/
void UnitDeathSwitch::onUnitWasKilled(ObjectIdType id) {
	if (this->doesUnitDeathCount(id)) {
		--this->requiredDeaths;
		if (this->requiredDeaths == 0) {
			this->setConditionState(true);
		}
	}
}

bool UnitDeathSwitch::doesUnitDeathCount(ObjectIdType id) {
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
