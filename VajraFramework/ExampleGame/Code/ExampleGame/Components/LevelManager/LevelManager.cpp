//
//  LevelManager.cpp
//  Created by Matt Kaufmann on 01/10/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/LevelManager/LevelLoader.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"
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
		case MESSAGE_TYPE_PAUSE:
			this->isPaused = true;
			break;
		case MESSAGE_TYPE_UNPAUSE:
			this->isPaused = false;		
		case MESSAGE_TYPE_LEVEL_UNLOADED:
			if(this->levelToLoad != -1) {
				ENGINE->GetMessageHub()->SendPointcastMessage(MESSAGE_TYPE_LOAD_LEVEL, this->GetObject()->GetId());
			}
			break;
		case MESSAGE_TYPE_LOAD_LEVEL:
				this->loadLevel_internal();
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
	this->isPaused = true;
	ENGINE->GetMessageHub()->SendMulticastMessage(MESSAGE_TYPE_LEVEL_UNLOADED);
}

void LevelManager::ReloadCurrentLevel() {
	this->LoadLevel(this->currentLevelIndex);
}

bool LevelManager::TryLoadNextLevel() {
	if(this->currentLevelIndex + 1 < this->levelData.size()) {
		this->LoadLevel(this->currentLevelIndex + 1);
		return true;
	}
	return false;
}

void LevelManager::loadLevel_internal() {
	ASSERT(this->levelToLoad < this->levelData.size(), "level number is less than the number of levels");
	if(this->levelToLoad < this->levelData.size()) {
		this->currentLevelIndex = this->levelToLoad;
		this->LoadLevelFromData(levelData[this->levelToLoad]);
		this->isPaused = false;
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

void LevelManager::init() {
	//this->shadyCam = nullptr;
	this->isPaused = false;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_PAUSE, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_UNPAUSE, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_LEVEL_UNLOADED, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_LOAD_LEVEL, this->GetTypeId(), false);

	this->levelToLoad = -1;

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
