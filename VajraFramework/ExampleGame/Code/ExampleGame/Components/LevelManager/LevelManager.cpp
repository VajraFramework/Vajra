//
//  LevelManager.cpp
//  Created by Matt Kaufmann on 01/10/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/LevelManager/LevelLoader.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
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
		case MESSAGE_TYPE_PAUSE:
			this->isPaused = true;
			break;
		case MESSAGE_TYPE_UNPAUSE:
			this->isPaused = false;
			break;
	}
}

void LevelManager::LoadLevel(int levelNumber) {
	ASSERT(levelNumber < this->levelData.size(), "level number is less than the number of levels");
	if(levelNumber < this->levelData.size()) {
		this->LoadLevelFromData(levelData[levelNumber]);
	}
}
void LevelManager::LoadLevelFromData(LevelData levelData) {
	this->currentLevelName = LevelLoader::LoadLevelFromFile(levelData.path);
	if(levelData.hasTutorial) {
		LevelLoader::LoadTutorialData(this->currentLevelName);
	}
}

void LevelManager::init() {
	//this->shadyCam = nullptr;
	this->isPaused = false;
	this->currentLevelName = "";

	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_PAUSE, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_UNPAUSE, this->GetTypeId(), false);

	// load the list of levels with a tutorial
	LevelLoader::LoadLevelData(&this->levelData);
}

void LevelManager::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void LevelManager::update() {
	if (!this->isPaused) {

	}
}

void LevelManager::endLevel(bool /*success*/) {
	this->isPaused = true;
	//LevelEnd.EndLevel(success);
}
