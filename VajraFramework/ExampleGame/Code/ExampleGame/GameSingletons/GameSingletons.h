//
//  GameSingletons.h
//  Created by Matt Kaufmann on 01/03/14.
//

#ifndef GAMESINGLETONS_H
#define GAMESINGLETONS_H

#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/LevelManager/LevelManager.h"

#include "Vajra/Common/Objects/Object.h"

class GameSingletons {
public:
	static GameSingletons* GetInstance();

	GameSingletons();
	~GameSingletons();

	inline Object* GetLevelManagerObject() { return this->levelManagerObj;  }
	inline LevelManager* GetLevelManager() { return this->levelManagerComp; }
	inline Object* GetGridManagerObject()  { return this->gridManagerObj;   }
	inline GridManager* GetGridManager()   { return this->gridManagerComp;  }

private:
	void init();
	void destroy();

	static GameSingletons* instance;

	Object* levelManagerObj;
	LevelManager* levelManagerComp;
	Object* gridManagerObj;
	GridManager* gridManagerComp;
};

#define SINGLETONS GameSingletons::GetInstance()

#endif // GAMESINGLETONS_H
