//
//  Guard.cpp
//  Created by Matt Kaufmann on 02/06/14.
//

#include "ExampleGame/Components/GameScripts/Units/Guard.h"

Guard::Guard() : EnemyUnit() {
	this->init();
}

Guard::Guard(Object* object_) : EnemyUnit(object_) {
	this->init();
}

Guard::~Guard() {
	this->destroy();
}

void Guard::init() {
	this->numPlayerUnitsSpotted = 0;
}

void Guard::destroy() {

}

void Guard::determineBrainState() {
	if (this->knownPlayers.size() == 0) {
		if (this->brainState != ENEMY_BRAIN_CALM) {
			this->brainState = ENEMY_BRAIN_CALM;
			this->routine->ResumeSchedule();
		}
	}
	else {
		this->brainState = ENEMY_BRAIN_CAUTIOUS;
	}
}
/*
void Guard::idleUpdate() {

}
*/
void Guard::cautiousUpdate() {

}

void Guard::aggressiveUpdate() {

}

void Guard::onSightedPlayerUnit(ObjectIdType id) {
	this->numPlayerUnitsSpotted++;
	this->knownPlayers.push_back(id);
}

void Guard::onLostSightOfPlayerUnit(ObjectIdType id) {
	this->numPlayerUnitsSpotted--;
	this->knownPlayers.remove(id);
}
