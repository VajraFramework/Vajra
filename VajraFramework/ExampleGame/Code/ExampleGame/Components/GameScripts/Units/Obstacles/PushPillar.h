//
//  PushPillar.h
//  Created by Matt Kaufmann on 03/31/14.
//

#ifndef PUSHPILLAR_H
#define PUSHPILLAR_H

#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"

//[[COMPONENT]]//
class PushPillar : public BaseUnit {
public:
	PushPillar();
	PushPillar(Object* object_);
	~PushPillar();

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	inline void SetSlideEffect(std::string prefabName);

	virtual bool CanBeKilledBy(ObjectIdType id, glm::vec3 source);

	static inline ComponentIdType GetTypeId()  { return BaseUnit::GetTypeId(); }

protected:
	virtual void start();
	virtual void end();
	virtual void update();

	virtual void onUnitSpecialHit(ObjectIdType id, int gridX, int gridZ, glm::vec3 source);

	virtual void Kill();

private:
	void init();
	void destroy();

	void generateSlideEffect();
	void activateSlideEffect();
	void deactivateSlideEffect();

	void startSliding(glm::vec3 direction);
	void setNextTarget();
	void slide();
	void changeCell(GridCell* goalCell, int elevation = -1);
	void stopSliding();

	void childUnitOnTop();
	void unchildUnitOnTop();

	void onZoneEnteredCell(int gridX, int gridZ);
	void onZoneExitedCell(int gridX, int gridZ);

	bool isSliding;
	int slideX, slideZ;
	glm::vec3 targetPosition;
	ObjectIdType riderId;

	std::string slideEffect;
	ObjectIdType slideEffectObjId;
};

void PushPillar::SetSlideEffect(std::string prefabName)  {
	this->slideEffect = prefabName;
	this->generateSlideEffect();
}

#endif // PUSHPILLAR_H
