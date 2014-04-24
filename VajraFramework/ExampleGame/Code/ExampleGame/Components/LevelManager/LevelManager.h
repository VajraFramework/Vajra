//
//  LevelManager.h
//  Created by Matt Kaufmann on 01/10/14.
//

#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

//#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Utilities/XmlParser/XmlParser.h"

#include "ExampleGame/Components/LevelManager/MasteryManager.h"
//#include "Vajra/Engine/Ui/UiElement/UiElement.h"

#include <string>

#include <vector>

// Defines for bundle data
#define PLAYER_BUNDLE_NAME "playerBundle"
#define LEVEL_COMPLETION "levelCompletionData"

#define MAX_LEVELS_PER_CONTRACT 15
#define MAX_LEVELS_POSSIBLE 256

class Bundle;
class LevelScores;

enum LevelType {
	Infiltration,
	Theft,
	Assassination,
	NO_TYPE
};

enum LevelCompletion {
	Locked,
	Unlocked,
	Completed,
	Completed_Bonus
};

struct LevelData {
public:
	std::string name;
	std::string path;
	std::string description;

	LevelType type;
	LevelCompletion completion;

	bool hasTutorial;
	int levelNum;

	int bounty;

	LevelBonus bonus;
	int bonusValue;

	int parallaxScreen;
	float pinX;
	float pinY;
};

struct MissionData {
public:
	std::string name;
	std::vector<LevelData*> levels;
};

struct ContractData {
public:
	std::string name;
	std::vector<MissionData*> missions;
};

#define LEVEL_BUNDLE_NAME "levelBundle_"
#define BONUS "bonus"
#define BEST_TAKE "bestTake"
#define BEST_TIME "bestTime"
#define BEST_KILL "bestKill"
#define BEST_ALERT "bestAlert"
#define BEST_LOOT "bestLoot"

//[[COMPONENT]]//
class LevelManager : public Component {
public:
	LevelManager();
	LevelManager(Object* object_);
	~LevelManager();

	void HandleMessage(MessageChunk messageChunk);

	inline std::string GetCurrentLevelName() { return this->currentLevelName; }
	inline int GetCurrentLevelIndex() { return this->currentLevelIndex; }

	//void LoadLevelFromAsset(std::string assetName); // Once we've got the loading process worked out, switch to using an asset
	void StartLevel();

	void AddWinCondition(ObjectIdType switchId);
	void AddLoseCondition(ObjectIdType switchId);

	inline int NumContracts() { return this->contractData.size(); }
	LevelData* GetLevelData(int missionIndex, int levelIndex);
	LevelData* GetLevelData(int contractIndex, int missionIndex, int levelIndex);
	MissionData* GetMissionData(int contractIndex, int missionIndex);
	ContractData* GetContractData(int index);

	inline int GetNumMissionsInCurrentContract() { return this->contractData[this->currentContract]->missions.size(); }
	inline int GetNumLevelsInCurrentMission() { return this->contractData[this->currentContract]->missions[this->currentMission]->levels.size(); }
	int GetNumLevelsInMission(int missionNum);

	inline int GetCurrentContract() { return this->currentContract; }
	void SetCurrentContract(int contractIndex) { this->currentContract = contractIndex; }
	inline int GetCurrentMission() { return this->currentMission; }
	void SetCurrentMission(int missionIndex) { this->currentMission = missionIndex; }

	void OnCurrentLevelWon(LevelCompletion completion);
	void SaveLevelScores(int levelIndex, LevelScores* scores);
private:
	void init();
	void destroy();
	void update();

	void onLevelUnlocked(int index);
	void loadLevelScore(int index);

	void UnloadLevel();
	bool TryLoadNextLevel();

	void ReloadCurrentLevel();
	void LoadLevel(int /*levelNumber*/);

	void loadLevel_internal();
	void LoadLevelFromData(LevelData* /*levelData*/);
	void clearEndConditions();
	/*****************
	bool playerHasWonLevel();
	bool playerHasLostLevel();
	*****************/

	void endLevel(bool success);
	LevelData* getNextLevel();

	static ComponentIdType componentTypeId;

	int currentLevelIndex;
	int currentMission;
	int currentContract;

	std::string currentLevelName;

	int levelToLoad;

	Object* winner;
	Object* loser;

	std::string levelCompletionData;

	std::vector<ContractData*> contractData;

	void initBundleForFirstTime();

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
