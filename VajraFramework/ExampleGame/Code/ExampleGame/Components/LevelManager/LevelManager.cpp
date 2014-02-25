//
//  LevelManager.cpp
//  Created by Matt Kaufmann on 01/10/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/LevelManager/LevelLoader.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/Components/Triggers/TriggerLevelDefeat.h"
#include "ExampleGame/Components/Triggers/TriggerLevelVictory.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"

#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Core/Engine.h"

#include <fstream>

ComponentIdType LevelManager::componentTypeId = COMPONENT_TYPE_ID_LEVEL_MANAGER;

LevelManager::LevelManager() : Component() {
	this->init();
}

LevelManager::LevelManager(Object* object_) : Component(object_) {
	this->init();
}

LevelManager::~LevelManager() {
	this->destroy();
}

void LevelManager::HandleMessage(MessageChunk messageChunk) {
	Component::HandleMessage(messageChunk);
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_FRAME_EVENT:
			this->update();
			break;
		case MESSAGE_TYPE_ON_END_CONDITIONS_MET:
			break;
		case MESSAGE_TYPE_LEVEL_UNLOADED:
			if(this->levelToLoad != -1) {
				MessageChunk mc = ENGINE->GetMessageHub()->GetOneFreeMessage();
				mc->SetMessageType(MESSAGE_TYPE_LOAD_LEVEL);
				// TODO [HACK We need this delay when loading a new level
				if(this->levelToLoad != this->currentLevelIndex) {
					mc->messageData.iv1.x = 3;
				} else {
					mc->messageData.iv1.x = 0;
				}
				ENGINE->GetMessageHub()->SendPointcastMessage(mc, this->GetObject()->GetId(), this->GetObject()->GetId());
			}
			break;
		case MESSAGE_TYPE_LOAD_LEVEL:
				if(messageChunk->messageData.iv1.x == 0) {
					this->loadLevel_internal();
				} else {
					MessageChunk mc = ENGINE->GetMessageHub()->GetOneFreeMessage();
					mc->SetMessageType(MESSAGE_TYPE_LOAD_LEVEL);
					mc->messageData.iv1.x = messageChunk->messageData.iv1.x - 1;
					ENGINE->GetMessageHub()->SendPointcastMessage(mc, this->GetObject()->GetId(), this->GetObject()->GetId());
				}
			break;
		default:
			break;
	}
}

void LevelManager::LoadLevel(int levelNumber) {
	this->levelToLoad = levelNumber;
	this->UnloadLevel();
}

void LevelManager::UnloadLevel() {
	// Unload the previous scene and all other items in the SceneGraph3D
	ENGINE->GetSceneGraph3D()->UnloadCurrentScene();
	this->clearEndConditions();
	ENGINE->GetMessageHub()->SendMulticastMessage(MESSAGE_TYPE_LEVEL_UNLOADED);
}

void LevelManager::ReloadCurrentLevel() {
	this->LoadLevel(this->currentLevelIndex);
}

bool LevelManager::TryLoadNextLevel() {
	if(this->currentLevelIndex + 1 < (int)this->levelData.size()) {
		this->LoadLevel(this->currentLevelIndex + 1);
		return true;
	}
	return false;
}

void LevelManager::loadLevel_internal() {
	ASSERT(this->levelToLoad < (int)this->levelData.size(), "level number is less than the number of levels");
	if(this->levelToLoad < (int)this->levelData.size()) {
		this->currentLevelIndex = this->levelToLoad;
		this->LoadLevelFromData(levelData[this->levelToLoad]);
		ENGINE->GetMessageHub()->SendMulticastMessage(MESSAGE_TYPE_LEVEL_LOADED);
	}
	this->levelToLoad = -1;
}

void LevelManager::LoadLevelFromData(LevelData levelData) {
	LevelLoader::LoadLevelFromFile(levelData.path);
	if(levelData.hasTutorial) {
		LevelLoader::LoadTutorialData(levelData.name);
	}
}

void LevelManager::AddWinCondition(ObjectIdType switchId) {
	Triggerable* victoryTrigger = this->winner->GetComponent<Triggerable>();
	victoryTrigger->SubscribeToSwitchObject(switchId);
}

void LevelManager::AddLoseCondition(ObjectIdType switchId) {
	Triggerable* defeatTrigger = this->loser->GetComponent<Triggerable>();
	defeatTrigger->SubscribeToSwitchObject(switchId);
}

void LevelManager::init() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_LEVEL_UNLOADED, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_LOAD_LEVEL, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_ON_END_CONDITIONS_MET, this->GetTypeId(), false);

	// Generate required end condition objects
	this->winner = new Object();
	winner->AddComponent<TriggerLevelVictory>();

	this->loser = new Object();
	loser->AddComponent<TriggerLevelDefeat>();

	this->levelToLoad = -1;

	// load the list of levels with a tutorial
	LevelLoader::LoadLevelData(&this->levelData);
}

void LevelManager::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());

	if (this->winner != nullptr) {
		delete this->winner;
		this->winner = nullptr;
	}

	if (this->loser != nullptr) {
		delete this->loser;
		this->loser = nullptr;
	}
}

void LevelManager::update() {
}

void LevelManager::endLevel(bool /*success*/) {
	//LevelEnd.EndLevel(success);
}

void LevelManager::clearEndConditions() {
	Triggerable* victoryTrigger = winner->GetComponent<TriggerLevelVictory>();
	if (victoryTrigger != nullptr) {
		victoryTrigger->UnsubscribeToAllSwitches();
	}

	Triggerable* defeatTrigger = loser->GetComponent<TriggerLevelDefeat>();
	if (defeatTrigger != nullptr) {
		defeatTrigger->UnsubscribeToAllSwitches();
	}
}
