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

	inline void SetCurrentBonuse(LevelBonus bonus) { this->currentBonus = bonus; }
private:
	void init();
	void destroy();

	virtual void start();

	float levelTime;
	int numKills;
	int money;
	int numAlerts;

	LevelBonus currentBonus;

	static ComponentIdType componentTypeId;
};

#endif // MASTERYMANAGER_H
