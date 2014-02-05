#ifndef UNIT_ANIMATION_MANAGER_H
#define UNIT_ANIMATION_MANAGER_H

#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "Vajra/Common/Components/Component.h"

/*
 * Manages the currently playing animation clip on the unit and
 * makes sure that it is appropriate for the unit's current action state
 */

class UnitAnimationManager : public Component {
public:
	UnitAnimationManager();
	UnitAnimationManager(Object* object_);
	~UnitAnimationManager();

	static inline ComponentIdType GetTypeId()  { return componentTypeId; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

private:
	void init();
	void destroy();

	void onUnitActionStateChanged(UnitActionState oldState, UnitActionState newState);

	static ComponentIdType componentTypeId;
};

#endif // UNIT_ANIMATION_MANAGER_H
