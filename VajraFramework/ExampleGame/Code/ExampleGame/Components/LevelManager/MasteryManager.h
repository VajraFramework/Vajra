#ifndef MASTERYMANAGER_H
#define MASTERYMANAGER_H

#include "Vajra/Common/Components/Component.h"

enum LevelBonus {
	None,
	Time,
	Kills,
	Money,
	Alerts
};

struct LevelScores {
	bool bonus;
	int time;
	int kills;
	int money;
	int alerts;
};

//[[COMPONENT]]//
class MasteryManager : public Component {
public:
	MasteryManager();
	MasteryManager(Object* object_);
	~MasteryManager();

	void HandleMessage(MessageChunk messageChunk);

	inline void AddMoney(int amt) { this->money += amt; }
	inline void OnGuardAlert() { this->numAlerts++; }

	inline void SetCurrentBonus(LevelBonus bonus, int bonusValue, int levelIndex) { this->currentBonus = bonus; this->bonusValue = bonusValue; this->currentLevelTracked = levelIndex; }
	LevelScores GetLevelScores(int levelIndex);
	inline float GetLevelTime() { return this->levelTime; }
	inline int GetNumKills() { return this->numKills; }
	inline int GetNumAlerts() { return this->numAlerts; }
	inline int GetMoney() { return this->money; }

	void ResetTracking();
	void OnLevelUnlocked(int index);

private:
	void init();
	void destroy();

	virtual void start();

	bool onLevelComplete();
	void onLevelUnlocked(int levelIndex, LevelScores scores);

	int currentLevelTracked;
	// Mastery Data to Track
	float levelTime;
	int numKills;
	int money;
	int numAlerts;

	// Current bonus 
	LevelBonus currentBonus;
	int bonusValue;

	std::vector<LevelScores> bestScores;

	static ComponentIdType componentTypeId;

	friend class LevelManager;
};

#endif // MASTERYMANAGER_H
