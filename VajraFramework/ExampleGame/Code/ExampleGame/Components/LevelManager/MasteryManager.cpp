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
	levelTime = 0.0f;
	numKills = 0;
	money = 0;
	numAlerts = 0;
}

void MasteryManager::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_500_MS_TIME_EVENT:
			levelTime += .5f;
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
				if(this->testBonusSucess()) {
					SINGLETONS->GetLevelManager()->OnCurrentLevelWon(LevelCompletion::Completed);
				} else {
					SINGLETONS->GetLevelManager()->OnCurrentLevelWon(LevelCompletion::Completed_Bonus);
				};
			}
			break;
		default:
			break;

	}
}

bool MasteryManager::testBonusSucess() {
	switch(this->currentBonus) {
		case Time:
			return levelTime <= this->bonusValue;
			break;
		case Kills:
			return numKills <= this->bonusValue;
			break;
		case Money:
			return money >= this->bonusValue;
			break;
		case Alerts:
			return numAlerts <= this->bonusValue;
			break;	
		default:
			break;
	}
	return false;
}
