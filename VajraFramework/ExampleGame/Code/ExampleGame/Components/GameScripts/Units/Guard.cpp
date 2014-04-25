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
#include "Vajra/Engine/Components/DerivedComponents/Audio/AudioSource.h"
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
#define ATTACK_RANGE 2.5f
// Angle at which the guard's attack can hit the player
#define ATTACK_ANGLE (15.0f inRadians)
// Guards can't be harmed if attacked from the front
#define INVINCIBLE_ANGLE (50.0f inRadians)

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
	this->targetId = OBJECT_ID_INVALID;
	this->weaponObjId = OBJECT_ID_INVALID;
	this->shieldObjId = OBJECT_ID_INVALID;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_ANIMATION_ENDED_EVENT, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SCENE_START, this->GetTypeId(), true);
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

void Guard::SwitchActionState(UnitActionState newState) {
	if(newState != this->GetUnitActionState()) {
		UnitActionState oldState = this->GetUnitActionState();
		MessageChunk messageChunk = ENGINE->GetMessageHub()->GetOneFreeMessage();
		messageChunk->SetMessageType(MESSAGE_TYPE_UNIT_ACTION_STATE_CHANGED);
		messageChunk->messageData.iv1.x = oldState;
		messageChunk->messageData.iv1.y = newState;
		if (this->weaponObjId != OBJECT_ID_INVALID) {
			ENGINE->GetMessageHub()->SendPointcastMessage(messageChunk, this->weaponObjId, this->GetObject()->GetId());
		}
		if (this->shieldObjId != OBJECT_ID_INVALID) {
			ENGINE->GetMessageHub()->SendPointcastMessage(messageChunk, this->shieldObjId, this->GetObject()->GetId());
		}
	}

	BaseUnit::SwitchActionState(newState);
}

bool Guard::CanBeKilledBy(ObjectIdType id, glm::vec3 /*source*/) {
	GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
	if (gObj != nullptr) {
		BaseUnit* unit = gObj->GetComponent<BaseUnit>();
		if (unit != nullptr) {
			if (unit->GetUnitType() == UNIT_TYPE_ASSASSIN) {
				// Guards can only be killed if they are unaware.
				auto iter = std::find(this->knownPlayers.begin(), this->knownPlayers.end(), id);
				return (iter == this->knownPlayers.end());
				/*
				// Guards are invincible from the front, so check the angle of the attack.
				glm::vec3 dir = source - this->gameObjectRef->GetTransform()->GetPositionWorld();
				glm::vec3 forward = QuaternionForwardVector(this->gameObjectRef->GetTransform()->GetOrientationWorld());
				// Don't use glm's vector angle function because it never returns an angle greater than PI/2
				float angle = acos(glm::clamp(glm::dot(dir, forward), -1.0f, 1.0f));
				if (angle >= INVINCIBLE_ANGLE) {
					return true;
				}
				*/
			}
		}
	}
	return false;
}

void Guard::start() {
	std::list<ObjectIdType> childIds = this->gameObjectRef->GetChildren();
	for (ObjectIdType childId : childIds) {
		GameObject* childObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(childId);
		if (childObj != nullptr) {
			if (childObj->HasTag("Weapon")) {
				this->weaponObjId = childId;
			}
			if (childObj->HasTag("Shield")) {
				this->shieldObjId = childId;
			}
		}
	}
}

void Guard::onBrainBecameCalm() {
	EnemyUnit::onBrainBecameCalm();
}

void Guard::onBrainBecameCautious() {
	EnemyUnit::onBrainBecameCautious();
	this->attackTimer = GUARD_ATTACK_TIME;
	this->cooldownTimer = GUARD_COOLDOWN_TIME;
	this->SwitchActionState(UNIT_ACTION_STATE_PRE_BLOCK);
	this->gridNavRef->HaltMovement();
}

void Guard::onBrainBecameAggressive() {
	EnemyUnit::onBrainBecameAggressive();
	this->SwitchActionState(UNIT_ACTION_STATE_DOING_SPECIAL);
}

void Guard::cautiousUpdate() {
	float dt = ENGINE->GetTimer()->GetDeltaFrameTime();

	this->attackTimer -= dt;

	if (this->knownPlayers.size() > 0) {
		this->cooldownTimer = GUARD_COOLDOWN_TIME;
	}
	else {
		this->targetId = OBJECT_ID_INVALID;
		this->cooldownTimer -= dt;
		if (this->cooldownTimer <= 0.0f) {
			this->setBrainState(ENEMY_BRAIN_CALM);
		}
	}

	glm::vec3 myLocation = this->GetObject()->GetComponent<Transform>()->GetPositionWorld();
	float minDistance = 100.0f;
	GameObject* targetObj = nullptr;
	for (auto iter = this->knownPlayers.begin(); iter != this->knownPlayers.end(); ++iter) {
		GameObject* playerObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(*iter);
		glm::vec3 unitLocation = playerObj->GetTransform()->GetPositionWorld();
		float distance = glm::distance(myLocation, unitLocation);
		if (distance < minDistance) {
			this->targetId = *iter;
			targetObj = playerObj;
			minDistance = distance;
		}
	}

	if (targetObj != nullptr) {
		// Turn to face towards the target
		this->gridNavRef->SetLookTarget(targetObj->GetTransform()->GetPositionWorld());

		// Attack the target if it's in range or it's doing a special.
		BaseUnit* targetUnit = targetObj->GetComponent<BaseUnit>();
		if ((((minDistance <= ATTACK_RANGE) && (this->GetUnitActionState() == UNIT_ACTION_STATE_BLOCK_IDLE))
			|| (targetUnit->GetUnitActionState() == UNIT_ACTION_STATE_DOING_SPECIAL))
			&& (targetUnit->GetUnitState() == UnitState::UNIT_STATE_ALIVE))
		{
			this->setBrainState(ENEMY_BRAIN_AGGRESSIVE);
		}
	}
}

void Guard::aggressiveUpdate() {
	// Keep turning towards the target
	GameObject* targetObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->targetId);
	if (targetObj != nullptr) {
		this->gridNavRef->SetLookTarget(targetObj->GetTransform()->GetPositionWorld());
	}
}

void Guard::onSightedPlayerUnit(ObjectIdType id) {
	if (this->GetUnitState() == UNIT_STATE_ALIVE) {
		this->numPlayerUnitsSpotted++;
		this->knownPlayers.push_back(id);

		if ((this->brainState == ENEMY_BRAIN_CALM) && (this->numPlayerUnitsSpotted > 0)) {
			this->setBrainState(ENEMY_BRAIN_CAUTIOUS);
		}
	}
}

void Guard::onLostSightOfPlayerUnit(ObjectIdType id) {
	if (this->GetUnitState() == UNIT_STATE_ALIVE) {
		this->numPlayerUnitsSpotted--;
		this->knownPlayers.remove(id);
	}
}

void Guard::onAnimationEnded(std::string animName) {
	if (animName == UNIT_ANIMATION_CLIP_NAME_block) {
		this->SwitchActionState(UNIT_ACTION_STATE_BLOCK_IDLE);
	}
	else if (animName == UNIT_ANIMATION_CLIP_NAME_doingspecial) {
		this->SwitchActionState(UNIT_ACTION_STATE_POST_SPECIAL);
		this->performAttack();
	}
	else if (animName == UNIT_ANIMATION_CLIP_NAME_postspecial) {
		this->setBrainState(ENEMY_BRAIN_CAUTIOUS);
	}
}

void Guard::performAttack() {
	GameObject* targetObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->targetId);
	if (targetObj != nullptr) {
		// Verify that the target is within reach
		glm::vec3 myPosition = this->gameObjectRef->GetTransform()->GetPositionWorld();
		glm::vec3 targetPosition = targetObj->GetTransform()->GetPositionWorld();
		float dist = glm::distance(targetPosition, myPosition);
		glm::vec3 forward = QuaternionForwardVector(this->gameObjectRef->GetTransform()->GetOrientationWorld());
		// Don't use glm's vector angle function because it never returns an angle greater than PI/2
		float angle = acos(glm::clamp(glm::dot(forward, targetPosition - myPosition), -1.0f, 1.0f));
		if ((dist <= ATTACK_RANGE) && (angle <= ATTACK_ANGLE)) {
			GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(targetPosition);
			if (cell != nullptr) {
				int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(myPosition.y);

				// Attack the intended target
				MessageChunk attackMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
				attackMessage->SetMessageType(MESSAGE_TYPE_UNIT_SPECIAL_HIT);
				attackMessage->messageData.iv1.x = cell->x;
				attackMessage->messageData.iv1.y = elevation;
				attackMessage->messageData.iv1.z = cell->z;
				attackMessage->messageData.fv1 = this->gameObjectRef->GetTransform()->GetPositionWorld();
				ENGINE->GetMessageHub()->SendMulticastMessage(attackMessage, this->GetObject()->GetId());

				AudioSource* audioSource = this->gameObjectRef->GetComponent<AudioSource>();
				if (audioSource != nullptr) {
					audioSource->Play("specialStart");
				}
			}
		}
	}
}
