//
//  LevelManager.h
//  Created by Matt Kaufmann on 01/10/14.
//

#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

//#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Utilities/XmlParser/XmlParser.h"
//#include "Vajra/Engine/Ui/UiElement/UiElement.h"

#include <string>

#include <vector>

enum LevelType {
	Infiltration,
	Theft,
	Assassination,
	NO_TYPE
};

struct LevelData {
public:
	std::string name;
	std::string path;
	std::string description;
	LevelType type;
	bool hasTutorial;
};

//[[COMPONENT]]//
class LevelManager : public Component {
public:
	LevelManager();
	LevelManager(Object* object_);
	~LevelManager();

	void HandleMessage(MessageChunk messageChunk);

	inline std::string GetCurrentLevelName() { return this->levelData[this->currentLevelIndex].name; }
	inline int GetCurrentLevelIndex() { return this->currentLevelIndex; }

	//void LoadLevelFromAsset(std::string assetName); // Once we've got the loading process worked out, switch to using an asset

	void AddWinCondition(ObjectIdType switchId);
	void AddLoseCondition(ObjectIdType switchId);

	inline bool IsPaused() { return this->isPaused; }
	inline int NumLevels() { return this->levelData.size(); }
private:
	void init();
	void destroy();
	void update();

	void UnloadLevel();
	bool TryLoadNextLevel();

	void ReloadCurrentLevel();
	void LoadLevel(int /*levelNumber*/);

	void loadLevel_internal();
	void LoadLevelFromData(LevelData /*levelData*/);
	void clearEndConditions();
	/*****************
	bool playerHasWonLevel();
	bool playerHasLostLevel();
	*****************/

	void endLevel(bool success);

	static ComponentIdType componentTypeId;

	//ShadyCamera* shadyCam;
	bool isPaused;
	int currentLevelIndex;
	int levelToLoad;

	Object* winner;
	Object* loser;

	std::vector<LevelData> levelData;

	//UiElement* levelTutorial;

	/*****************
	These are all from the Unity prototype but will probably be deprecated
	******************
	std::vector<EndCondition*> winCons; // Probably deprecate this
	std::vector<EndCondition*> loseCons; // This too
	int PLAYER_THREATS;
	*****************/

	friend class MenuManager;
};

#endif // LEVELMANAGER_H
