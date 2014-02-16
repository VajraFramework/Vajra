#include "ExampleGame/Components/Switches/GoalZoneSwitch.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Common/Objects/Object.h"

GoalZoneSwitch::GoalZoneSwitch() : GridZoneSwitch() {
	this->init();
}

GoalZoneSwitch::GoalZoneSwitch(Object* object_) : GridZoneSwitch(object_) {
	this->init();
}

GoalZoneSwitch::~GoalZoneSwitch() {
	this->destroy();
}

void GoalZoneSwitch::init() {
}

void GoalZoneSwitch::destroy() {
}

bool GoalZoneSwitch::doesObjectCountAsOccupant(ObjectIdType /*id*/) {
	return true;
}
