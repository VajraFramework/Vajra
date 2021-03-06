//
//  EnemyUnit.cpp
//  Created by Matt Kaufmann on 01/14/14.
//

#include "ExampleGame/Components/GameScripts/Units/EnemyUnit.h"
#include "ExampleGame/Components/LevelManager/MasteryManager.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"

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
	//this->navigator = this->GetObject()->GetComponent<GridNavigator>();
	//ASSERT(this->navigator != nullptr, "Object with EnemyUnit also has required component GridNavigator");
	this->isActive = true;
}

void EnemyUnit::HandleMessage(MessageChunk messageChunk) {
	BaseUnit::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_AI_SIGHTED_PLAYER:
			this->onSightedPlayerUnit(messageChunk->messageData.iv1.x);
			break;

		case MESSAGE_TYPE_AI_LOST_SIGHT_OF_PLAYER:
			this->onLostSightOfPlayerUnit(messageChunk->messageData.iv1.x);
			break;
	}
}

void EnemyUnit::update() {
	if (this->isActive && (this->GetUnitState() == UnitState::UNIT_STATE_ALIVE)) {
		switch (this->brainState) {
			case ENEMY_BRAIN_CALM:
				this->idleUpdate();
				break;

			case ENEMY_BRAIN_CAUTIOUS:
				this->cautiousUpdate();
				break;

			case ENEMY_BRAIN_AGGRESSIVE:
				this->aggressiveUpdate();
				break;
		}
	}
}

void EnemyUnit::setBrainState(EnemyBrainState bState) {
	if (this->brainState != bState) {
		switch (bState) {
			case ENEMY_BRAIN_CALM:
				this->onBrainBecameCalm();
				break;

			case ENEMY_BRAIN_CAUTIOUS:
				this->onBrainBecameCautious();
				break;

			case ENEMY_BRAIN_AGGRESSIVE:
				this->onBrainBecameAggressive();
				break;

			default:
				break;
		}
		this->brainState = bState;
	}
}

void EnemyUnit::onBrainBecameCalm() {
	this->routine->ResumeSchedule();
}

void EnemyUnit::onBrainBecameCautious() {
	SINGLETONS->GetMasteryManager()->OnGuardAlert();	
}

void EnemyUnit::onBrainBecameAggressive() {

}

void EnemyUnit::Kill() {
	BaseUnit::Kill();
}

void EnemyUnit::idleUpdate() {
	this->routine->Follow();
}

void EnemyUnit::cautiousUpdate() {
	// If child classes don't implement this, it just tosses up to the idle state
	this->idleUpdate();
}

void EnemyUnit::aggressiveUpdate() {
	// If child classes don't impelement this, it just tosses up to the cautious state
	this->cautiousUpdate();
}

void EnemyUnit::init() {
	this->knowledge = nullptr;
	this->routine   = nullptr;
	//this->navigator = nullptr;
	this->isActive = false;
	this->alertness = 0.0f;
	this->brainState = ENEMY_BRAIN_CALM;
	this->gridNavRef->SetMaxNavigableUnitType(LAST_ENEMY_UNIT_TYPE);

	this->addSubscriptionToMessageType(MESSAGE_TYPE_AI_SIGHTED_PLAYER, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_AI_LOST_SIGHT_OF_PLAYER, this->GetTypeId(), false);
}

void EnemyUnit::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
