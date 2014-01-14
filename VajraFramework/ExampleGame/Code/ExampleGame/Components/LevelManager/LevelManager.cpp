//
//  LevelManager.cpp
//  Created by Matt Kaufmann on 01/10/14.
//

#include "ExampleGame/Components/LevelManager/LevelManager.h"
#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/LevelManager/LevelFileTags.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"

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
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_FRAME_EVENT:
			this->update();
			break;
	}
}

void LevelManager::LoadLevelFromFile(std::string levelFilename) {
	std::ifstream ifs;
	ifs.open(levelFilename, std::ios_base::in);

	ASSERT(!ifs.fail(), "Loading level data from file %s", levelFilename.c_str());

	std::string tag;
	ifs >> tag;
	ASSERT(tag == LEVEL_NAME_TAG, "Reading level name from file %s", levelFilename.c_str());
	ifs >> this->currentLevelName;

	SINGLETONS->GetGridManager()->loadGridDataFromStream(ifs);

	ifs.close();

	this->isPaused = false;
}

void LevelManager::init() {
	//this->shadyCam = nullptr;
	this->isPaused = true;
	this->currentLevelName = "";

	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
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
