#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"

ComponentIdType BaseUnit::componentTypeId = COMPONENT_TYPE_ID_BASE_UNIT;

BaseUnit::BaseUnit() : GameScript() {
	this->init();
}

BaseUnit::BaseUnit(Object* object_) : GameScript(object_) {
	this->init();
}

BaseUnit::~BaseUnit() {
	this->destroy();
}

void BaseUnit::init() {
	this->start();
}

void BaseUnit::destroy() {
	this->end();
}

void BaseUnit::start() {
	this->gridNavRef = this->GetObject()->GetComponent<GridNavigator>();
}

void BaseUnit::end() {
	this->gridNavRef = nullptr;
}

void BaseUnit::update() {

}

void BaseUnit::Kill() {
	this->unitState = UnitState::UNIT_STATE_DEAD;
	// Send a message to grid to remove this navigator
}