//
//  EnemyUnit.cpp
//  Created by Matt Kaufmann on 01/14/14.
//

#include "ExampleGame/Components/GameScripts/Units/EnemyUnit.h"

EnemyUnit::EnemyUnit() : BaseUnit() {
	this->init();
}

EnemyUnit::EnemyUnit(Object* object_) : BaseUnit(object_) {
	this->init();
}

EnemyUnit::~EnemyUnit() {
	this->destroy();
}

void EnemyUnit::Activate() {
	this->knowledge = this->GetObject()->GetComponent<AiKnowledge>();
	ASSERT(this->knowledge != nullptr, "Object with EnemyUnit also has required component AiKnowledge");
	this->routine   = this->GetObject()->GetComponent<AiRoutine>();
	ASSERT(this->routine != nullptr, "Object with EnemyUnit also has required component AiRoutine");
	this->navigator = this->GetObject()->GetComponent<GridNavigator>();
	ASSERT(this->navigator != nullptr, "Object with EnemyUnit also has required component GridNavigator");
	this->isActive = true;
}

void EnemyUnit::update() {
	if (this->isActive) {
		this->idleUpdate();
	}
}

void EnemyUnit::idleUpdate() {
	this->routine->Follow();
}

void EnemyUnit::init() {
	this->knowledge = nullptr;
	this->routine   = nullptr;
	this->navigator = nullptr;
	this->isActive = false;
}

void EnemyUnit::destroy() {

}
