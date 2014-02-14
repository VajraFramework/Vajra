#ifndef UNIT_SHADOW_H
#define UNIT_SHADOW_H

#include "Vajra/Common/Components/Component.h"

// Forward Declarations:
class GameObject;

//[[COMPONENT]]//
class UnitShadow : public Component {
public:
	UnitShadow();
	UnitShadow(Object* object_);
	~UnitShadow();

	static inline ComponentIdType GetTypeId()  { return componentTypeId; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

protected:
	// @Override
	virtual void start();

private:
	void init();
	void destroy();

	GameObject* gameObjectRef;
	GameObject* littleShadow;

	static ComponentIdType componentTypeId;
};

#endif // UNIT_SHADOW_H
