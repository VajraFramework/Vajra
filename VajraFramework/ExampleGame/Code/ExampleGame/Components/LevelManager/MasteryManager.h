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

//[[COMPONENT]]//
class MasteryManager : public Component {
public:
	MasteryManager();
	MasteryManager(Object* object_);
	~MasteryManager();

	void HandleMessage(MessageChunk messageChunk);

	inline void AddMoney(int amt) { this->money += amt; }
	inline void OnGuardAlert() { this->numAlerts++; }

	inline void SetCurrentBonuse(LevelBonus bonus, int bonusValue) { this->currentBonus = bonus; this->bonusValue = bonusValue;}

	inline int GetLevelTime() { return this->levelTime; }
	inline int GetNumKills() { return this->numKills; }
	inline int GetNumAlerts() { return this->levelTime; }
	inline int GetMoney() { return this->money; }
	
private:
	void init();
	void destroy();

	virtual void start();

	bool testBonusSucess();

	// Mastery Data to Track
	int levelTime;
	int numKills;
	int money;
	int numAlerts;

	// Current bonus 
	LevelBonus currentBonus;
	int bonusValue;

	static ComponentIdType componentTypeId;
};

#endif // MASTERYMANAGER_H
