#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"

PlayerUnit::PlayerUnit() : BaseUnit() {
	this->init();
}

PlayerUnit::PlayerUnit(Object* object_) : BaseUnit(object_) {
	this->init();
}

PlayerUnit::~PlayerUnit() {
	this->destroy();
}

void PlayerUnit::init() {
}

void PlayerUnit::destroy() {
}

void PlayerUnit::OnTouch(int touchId) {
	
}