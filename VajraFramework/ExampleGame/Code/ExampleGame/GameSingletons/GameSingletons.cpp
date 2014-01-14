//
//  GameSingletons.cpp
//  Created by Matt Kaufmann on 01/03/14.
//

#include "ExampleGame/GameSingletons/GameSingletons.h"

GameSingletons* GameSingletons::instance = nullptr;

GameSingletons* GameSingletons::GetInstance() {
	if (instance == nullptr) {
		instance = new GameSingletons();
		instance->init();
	}

	return instance;
}

GameSingletons::GameSingletons() :
	levelManagerObj(nullptr), levelManagerComp(nullptr),
	gridManagerObj(nullptr),  gridManagerComp(nullptr)
{
}

GameSingletons::~GameSingletons() {
	destroy();
}

void GameSingletons::init() {
	this->levelManagerObj = new Object();
	this->levelManagerComp = this->levelManagerObj->AddComponent<LevelManager>();
	this->gridManagerObj = new Object();
	this->gridManagerComp = this->gridManagerObj->AddComponent<GridManager>();
}

void GameSingletons::destroy() {
	if (this->levelManagerObj != nullptr) {
		delete this->levelManagerObj;
		this->levelManagerObj = nullptr;
	}

	if (this->gridManagerObj != nullptr) {
		delete this->gridManagerObj;
		this->gridManagerObj = nullptr;
	}
}
