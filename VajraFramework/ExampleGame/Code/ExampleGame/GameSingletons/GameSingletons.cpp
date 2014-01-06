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
	gridManagerObj(nullptr), gridManagerComp(nullptr)
{
}

GameSingletons::~GameSingletons() {
	destroy();
}

void GameSingletons::init() {
	this->gridManagerObj = new Object();
	this->gridManagerComp = this->gridManagerObj->AddComponent<GridManager>();
}

void GameSingletons::destroy() {
	if (this->gridManagerObj) {
		delete this->gridManagerObj;
		this->gridManagerObj = nullptr;
	}
}

