//
//  Guard.cpp
//  Created by Matt Kaufmann on 02/06/14.
//

#include "ExampleGame/Components/GameScripts/UnitCustomizers/Definitions.h"
#include "ExampleGame/Components/GameScripts/Units/Guard.h"
#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Libraries/glm/gtx/vector_angle.hpp"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Utilities/MathUtilities.h"

// Wait time to return to idle after losing sight of players
#define GUARD_COOLDOWN_TIME 1.0f
// Wait time for guards to attack
#define GUARD_ATTACK_TIME 1.0f
// Range within which guards will attack players
#define ATTACK_RANGE 1.5f
// Guards can't be harmed if attacked from the front
#define INVINCIBLE_ANGLE (PI / 3.5f)

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
	this->targetCell = nullptr;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_ANIMATION_ENDED_EVENT, this->GetTypeId(), true);
}

void Guard::destroy() {

}

void Guard::HandleMessage(MessageChunk messageChunk) {
	EnemyUnit::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_ANIMATION_ENDED_EVENT:
			this->onAnimationEnded(messageChunk->messageData.s);
			break;
	}
}

bool Guard::CanBeKilledBy(ObjectIdType id, glm::vec3 source) {
	GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
	if (gObj != nullptr) {
		BaseUnit* unit = gObj->GetComponent<BaseUnit>();
		if (unit != nullptr) {
			if ((unit->GetUnitType() >= FIRST_PLAYER_UNIT_TYPE) && (unit->GetUnitType() <= LAST_PLAYER_UNIT_TYPE)) {
				// Guards are invincible from the front, so check the angle of the attack.
				glm::vec3 dir = source - this->gameObjectRef->GetTransform()->GetPositionWorld();
				glm::vec3 forward = QuaternionForwardVector(this->gameObjectRef->GetTransform()->GetOrientationWorld());
				float angle = glm::angle(dir, forward);
				if (angle >= INVINCIBLE_ANGLE) {
					return true;
				}
			}
		}
	}
	return false;
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
}

void Guard::onBrainBecameCalm() {
	this->SwitchActionState(UNIT_ACTION_STATE_IDLE);
}

void Guard::onBrainBecameCautious() {
	this->attackTimer = GUARD_ATTACK_TIME;
	this->cooldownTimer = GUARD_COOLDOWN_TIME;
	this->isTargetInRange = false;
	this->SwitchActionState(UNIT_ACTION_STATE_PRE_SPECIAL);
}

void Guard::onBrainBecameAggressive() {
	this->SwitchActionState(UNIT_ACTION_STATE_DOING_SPECIAL);
}

void Guard::cautiousUpdate() {
	float dt = ENGINE->GetTimer()->GetDeltaFrameTime();

	this->attackTimer -= dt;

	if (this->knownPlayers.size() > 0) {
		this->cooldownTimer = GUARD_COOLDOWN_TIME;
	}
	else {
		this->cooldownTimer -= dt;
		if (this->cooldownTimer <= 0.0f) {
			this->setBrainState(ENEMY_BRAIN_CALM);
		}
	}

	if (this->attackTimer <= 0.0f) {
		glm::vec3 myLocation = this->GetObject()->GetComponent<Transform>()->GetPositionWorld();
		for (auto iter = this->knownPlayers.begin(); iter != this->knownPlayers.end(); ++iter) {
			GameObject* playerObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(*iter);
			glm::vec3 unitLocation = playerObj->GetTransform()->GetPositionWorld();
			float distance = glm::distance(myLocation, unitLocation);
			if (distance <= ATTACK_RANGE) {
				isTargetInRange = true;
				this->setBrainState(ENEMY_BRAIN_AGGRESSIVE);
				this->targetCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(unitLocation);
			}
		}
	}
}

void Guard::aggressiveUpdate() {

}

void Guard::onSightedPlayerUnit(ObjectIdType id) {
	this->numPlayerUnitsSpotted++;
	this->knownPlayers.push_back(id);

	if ((this->brainState == ENEMY_BRAIN_CALM) && (this->numPlayerUnitsSpotted > 0)) {
		this->setBrainState(ENEMY_BRAIN_CAUTIOUS);
	}
}

void Guard::onLostSightOfPlayerUnit(ObjectIdType id) {
	this->numPlayerUnitsSpotted--;
	this->knownPlayers.remove(id);
}

void Guard::onAnimationEnded(std::string animName) {
	if (animName == UNIT_ANIMATION_CLIP_NAME_doingspecial) {
		this->SwitchActionState(UNIT_ACTION_STATE_POST_SPECIAL);
	}
	else if (animName == UNIT_ANIMATION_CLIP_NAME_postspecial) {
		if (this->targetCell != nullptr) {
			// Attack the intended target
			MessageChunk attackMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			attackMessage->SetMessageType(MESSAGE_TYPE_UNIT_SPECIAL_HIT);
			attackMessage->messageData.iv1.x = this->targetCell->x;
			attackMessage->messageData.iv1.y = this->targetCell->y;
			attackMessage->messageData.iv1.z = this->targetCell->z;
			attackMessage->messageData.fv1 = this->gameObjectRef->GetTransform()->GetPositionWorld();
			ENGINE->GetMessageHub()->SendMulticastMessage(attackMessage, this->GetObject()->GetId());
		}
		this->setBrainState(ENEMY_BRAIN_CAUTIOUS);
	}
}
