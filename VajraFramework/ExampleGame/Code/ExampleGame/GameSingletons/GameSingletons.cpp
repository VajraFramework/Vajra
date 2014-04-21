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
	gridManagerObj(nullptr),  gridManagerComp(nullptr),
	menuManagerObj(nullptr),  menuManagerComp(nullptr),
	masteryManagerObj(nullptr),  masteryManagerComp(nullptr)
{
}

GameSingletons::~GameSingletons() {
	destroy();
}

void GameSingletons::init() {
	this->masteryManagerObj = new Object();
	this->masteryManagerComp = this->masteryManagerObj->AddComponent<MasteryManager>();
	this->levelManagerObj = new Object();
	this->levelManagerComp = this->levelManagerObj->AddComponent<LevelManager>();
	this->gridManagerObj = new Object();
	this->gridManagerComp = this->gridManagerObj->AddComponent<GridManager>();
	this->menuManagerObj = new Object();
	this->menuManagerComp = this->menuManagerObj->AddComponent<MenuManager>();
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
	
	if (this->menuManagerObj != nullptr) {
		delete this->menuManagerObj;
		this->menuManagerObj = nullptr;
	}
	
	if (this->masteryManagerObj != nullptr) {
		delete this->masteryManagerObj;
		this->masteryManagerObj = nullptr;
	}
}
