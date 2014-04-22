#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/Components/LevelManager/MasteryManager.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"

ComponentIdType MasteryManager::componentTypeId = COMPONENT_TYPE_ID_MASTERY_MANAGER;

MasteryManager::MasteryManager() : Component() {
	this->init();
}

MasteryManager::MasteryManager(Object* object_) : Component(object_) {
	this->init();
}

MasteryManager::~MasteryManager() {
	this->destroy();
}

void MasteryManager::init() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SCENE_START, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_500_MS_TIME_EVENT, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_UNIT_KILLED, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_ON_END_CONDITIONS_MET, this->GetTypeId(), false);


}

void MasteryManager::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void MasteryManager::start() {
	this->ResetTracking();
}

void MasteryManager::ResetTracking() {
	this->levelTime = 0.0f;
	this->numKills = 0;
	this->money = 0;
	this->numAlerts = 0;
}

void MasteryManager::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_500_MS_TIME_EVENT:
			if(!ENGINE->GetSceneGraph3D()->IsPaused()) {
				this->levelTime += 0.5;
			}
			break;
		case MESSAGE_TYPE_UNIT_KILLED:
			{
				Object* sendObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(messageChunk->GetSenderId());
				if(sendObj != nullptr) {
					BaseUnit* unit = sendObj->GetComponent<BaseUnit>();
					if(unit != nullptr && ( unit->GetUnitType() >= FIRST_ENEMY_UNIT_TYPE && unit->GetUnitType() <= LAST_ENEMY_UNIT_TYPE)) {
						numKills++;
					}
				}
			}
			break;

		case MESSAGE_TYPE_ON_END_CONDITIONS_MET:
			// if the level was won
			if(messageChunk->messageData.iv1.x >= 0) {
				if(this->onLevelComplete()) {
					SINGLETONS->GetLevelManager()->OnCurrentLevelWon(LevelCompletion::Completed_Bonus);
				} else {
					SINGLETONS->GetLevelManager()->OnCurrentLevelWon(LevelCompletion::Completed);
				};
			}
			break;
		default:
			break;

	}
}

LevelScores MasteryManager::GetLevelScores(int levelIndex) {
	VERIFY((int)this->bestScores.size() > levelIndex, " the level has a score");
	return this->bestScores[levelIndex];
}
bool MasteryManager::onLevelComplete() {
	// update high scores
	if((int)this->bestScores.size() > this->currentLevelTracked) {
		LevelScores scores = this->bestScores[this->currentLevelTracked];
		if(scores.time > levelTime || scores.time == -1) {
			scores.time = levelTime;
		}
		if(scores.kills > numKills || scores.kills == -1) {
			scores.kills = numKills;
		}
		if(scores.money < money || scores.money == -1) {
			scores.money = money;
		}
		if(scores.alerts > numAlerts || scores.alerts == -1) {
			scores.alerts = numAlerts;
		}

		if(!scores.bonus) {
			switch(this->currentBonus) {
			case Time:
				scores.bonus = levelTime <= this->bonusValue;
				break;
			case Kills:
				scores.bonus = numKills <= this->bonusValue;
				break;
			case Money:
				scores.bonus = money >= this->bonusValue;
				break;
			case Alerts:
				scores.bonus = numAlerts <= this->bonusValue;
				break;	
			default:
				break;
			}
		}

		this->bestScores[this->currentLevelTracked] = scores;
		SINGLETONS->GetLevelManager()->SaveLevelScores(this->currentLevelTracked, &scores);
		return scores.bonus;
	}
	ASSERT(false, "current level has a bundle");
	return false;
}


void MasteryManager::updateLevelScore(int levelIndex, LevelScores scores) {
	if((int)this->bestScores.size() == levelIndex) {
		this->bestScores.push_back(scores);
	} else if ((int)this->bestScores.size() > levelIndex) {
		this->bestScores[levelIndex] = scores;
	}
}
