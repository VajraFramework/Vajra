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

void EnemyUnit::update() {

}

void EnemyUnit::init() {

}

void EnemyUnit::destroy() {

}
