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
	this->routine   = this->GetObject()->GetComponent<AiRoutine>();
	this->navigator = this->GetObject()->GetComponent<GridNavigator>();
}

void EnemyUnit::update() {
	this->idleUpdate();
}

void EnemyUnit::idleUpdate() {
	this->routine->Follow();
}

void EnemyUnit::init() {
	this->knowledge = nullptr;
	this->routine   = nullptr;
	this->navigator = nullptr;
}

void EnemyUnit::destroy() {

}
