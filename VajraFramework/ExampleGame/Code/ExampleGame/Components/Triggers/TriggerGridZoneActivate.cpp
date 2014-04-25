//
//  TriggerGridZoneActivate.cpp
//  Created by Matt Kaufmann on 04/24/14.
//

#include "ExampleGame/Components/Grid/GridZone.h"
#include "ExampleGame/Components/Triggers/TriggerGridZoneActivate.h"
#include "Vajra/Common/Objects/Object.h"

TriggerGridZoneActivate::TriggerGridZoneActivate() : Triggerable() {
	this->init();
}

TriggerGridZoneActivate::TriggerGridZoneActivate(Object* object_) : Triggerable(object_) {
	this->init();
}

TriggerGridZoneActivate::~TriggerGridZoneActivate() {
	this->destroy();
}

void TriggerGridZoneActivate::SetTriggerType(std::string typeStr) {
	Triggerable::SetTriggerType(typeStr);
}

void TriggerGridZoneActivate::SubscribeToMySwitch() {
	Triggerable::SubscribeToMySwitch();
}

void TriggerGridZoneActivate::SubscribeToParentSwitch() {
	Triggerable::SubscribeToParentSwitch();
}

void TriggerGridZoneActivate::onSwitchToggled(bool switchState) {
	GridZone* zone = this->GetObject()->GetComponent<GridZone>();
	if (zone != nullptr) {
		if (zone->IsEnabled()) {
			zone->Disable();
		}
		else {
			zone->Enable();
		}
	}
}

void TriggerGridZoneActivate::init() {

}

void TriggerGridZoneActivate::destroy() {

}
