//
//  BreakablePot.h
//  Created by Matt Kaufmann on 02/22/14.
//

#ifndef BREAKABLEPOT_H
#define BREAKABLEPOT_H

#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"

#include <string>

#define LINK_NONE      0x00
#define LINK_NORTH     0x01
#define LINK_EAST      0x02
#define LINK_SOUTH     0x04
#define LINK_WEST      0x08
#define LINK_NORTHEAST 0x10
#define LINK_SOUTHEAST 0x20
#define LINK_SOUTHWEST 0x40
#define LINK_NORTHWEST 0x80

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

	//[[PROPERTY]]//
	void AddLinkage(std::string dirStr);
	void AddLinkage(unsigned char dir);

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

	void chainReaction();
	void checkChainAtCell(int x, int z, int elevation);

	std::string deathEffect;
	ObjectIdType deathEffectObjId;

	ObjectIdType potLootId;
	unsigned char linkages;
};

void BreakablePot::SetDeathEffect(std::string prefabName)  {
	this->deathEffect = prefabName;
	this->generateDeathEffect();
}

#endif // BREAKABLEPOT_H
