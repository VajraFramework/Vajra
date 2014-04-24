#ifndef MASTERYMANAGER_H
#define MASTERYMANAGER_H

#include "Vajra/Common/Components/Component.h"

enum LevelBonus {
	Time,
	Kills,
	Money,
	Alerts, 
	None
};

struct LevelScores {
	bool bonus;
	int take;
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

	void AddMoney(int amt);
	inline void OnGuardAlert() { this->numAlerts++; }

	inline void SetCurrentBonus(LevelBonus bonus, int bonusValue, int levelIndex, int bounty) { this->currentBonus = bonus; this->bonusValue = bonusValue; this->currentLevelTracked = levelIndex; this->currentLevelBounty = bounty; }
	LevelScores GetLevelScores(int levelIndex);
	inline float GetLevelTime() { return this->levelTime; }
	inline int GetNumKills() { return this->numKills; }
	inline int GetNumAlerts() { return this->numAlerts; }
	inline int GetMoney() { return this->money; }

	void ResetTracking();
	void OnLevelUnlocked(int index);

	LevelBonus GetCurrentBonusType() { return this->currentBonus; }

	LevelScores GetCurrentLevelTakeScores() { return this->takeScores;}

private:
	void init();
	void destroy();

	virtual void start();

	bool onLevelComplete();
	void updateLevelScore(int levelIndex, LevelScores scores);

	int currentLevelTracked;
	int currentLevelBounty;
	// Mastery Data to Track
	float levelTime;
	int take;
	int numKills;
	int money;
	int numAlerts;

	void calculateTake();
	// Current bonus 
	LevelBonus currentBonus;
	int bonusValue;

	LevelScores takeScores;

	std::vector<LevelScores> bestScores;

	static ComponentIdType componentTypeId;

	friend class LevelManager;
};

#endif // MASTERYMANAGER_H
