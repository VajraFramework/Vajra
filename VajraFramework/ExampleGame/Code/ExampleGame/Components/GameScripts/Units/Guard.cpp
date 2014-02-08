//
//  Guard.cpp
//  Created by Matt Kaufmann on 02/06/14.
//

#include "ExampleGame/Components/GameScripts/Units/Guard.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Timer/Timer.h"

// Wait time to return to idle after losing sight of players
#define GUARD_COOLDOWN_TIME 1.0f
// Wait time for guards to attack
#define GUARD_ATTACK_TIME 1.0f
// Range within which guards will attack players
#define ATTACK_RANGE 1.5f

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
	this->cooldownTimer = GUARD_COOLDOWN_TIME;
	this->attackTimer = GUARD_ATTACK_TIME;
	this->isTargetInRange = false;
	this->targetId = OBJECT_ID_INVALID;
}

void Guard::destroy() {

}

void Guard::determineBrainState() {
	if (this->brainState == ENEMY_BRAIN_CALM) {
		if (this->knownPlayers.size() > 0) {
			this->setBrainState(ENEMY_BRAIN_CAUTIOUS);
		}
	}
	else if (this->brainState == ENEMY_BRAIN_CAUTIOUS) {
		if (this->cooldownTimer <= 0.0f) {
			this->setBrainState(ENEMY_BRAIN_CALM);
		}
		else if (this->isTargetInRange) {
			this->setBrainState(ENEMY_BRAIN_AGGRESSIVE);
		}
	}
	else if (this->brainState == ENEMY_BRAIN_AGGRESSIVE) {

	}
}

void Guard::setBrainState(EnemyBrainState bState) {
	switch (bState) {
		case ENEMY_BRAIN_CALM:
			this->routine->ResumeSchedule();
			break;

		case ENEMY_BRAIN_CAUTIOUS:
			this->attackTimer = GUARD_ATTACK_TIME;
			this->cooldownTimer = GUARD_COOLDOWN_TIME;
			this->isTargetInRange = false;
			break;

		case ENEMY_BRAIN_AGGRESSIVE:
			this->SwitchActionState(UNIT_ACTION_STATE_DOING_SPECIAL);
			break;

		default:
			break;
	}

	this->brainState = bState;
}
/*
void Guard::idleUpdate() {

}
*/
void Guard::cautiousUpdate() {
	float dt = ENGINE->GetTimer()->GetDeltaFrameTime();

	this->attackTimer -= dt;

	if (this->knownPlayers.size() > 0) {
		this->cooldownTimer = GUARD_COOLDOWN_TIME;
	}
	else {
		this->cooldownTimer -= dt;
	}

	if (this->attackTimer <= 0.0f) {
		glm::vec3 myLocation = this->GetObject()->GetComponent<Transform>()->GetPositionWorld();
		for (auto iter = this->knownPlayers.begin(); iter != this->knownPlayers.end(); ++iter) {
			GameObject* playerObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(*iter);
			glm::vec3 unitLocation = playerObj->GetTransform()->GetPositionWorld();
			float distance = glm::distance(myLocation, unitLocation);
			if (distance <= ATTACK_RANGE) {
				isTargetInRange = true;
				this->targetId = *iter;
			}
		}
	}
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
