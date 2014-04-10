//
//  BreakablePot.h
//  Created by Matt Kaufmann on 02/22/14.
//

#ifndef BREAKABLEPOT_H
#define BREAKABLEPOT_H

#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"

#include <string>

//[[COMPONENT]]//
class BreakablePot : public BaseUnit {
public:
	BreakablePot();
	BreakablePot(Object* object_);
	~BreakablePot();

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	inline void SetDeathEffect(std::string prefabName);

	//[[PROPERTY]]//
	void SetHasPotLoot();

	virtual bool CanBeKilledBy(ObjectIdType id, glm::vec3 source);

	static inline ComponentIdType GetTypeId()  { return BaseUnit::GetTypeId(); }

protected:
	virtual void start();
	virtual void end();
	virtual void update();

	// @Override
	virtual void Kill();

private:
	void init();
	void destroy();

	void generateDeathEffect();
	void activateDeathEffect();

	void tryLoot();

	std::string deathEffect;
	ObjectIdType deathEffectObjId;

	ObjectIdType potLootId;
};

void BreakablePot::SetDeathEffect(std::string prefabName)  {
	this->deathEffect = prefabName;
	this->generateDeathEffect();
}

#endif // BREAKABLEPOT_H
