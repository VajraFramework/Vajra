#ifndef SHADOW_MAP_UPDATE_LISTENER_H
#define SHADOW_MAP_UPDATE_LISTENER_H

#include "Vajra/Common/Components/Component.h"

// Forward Declarations:
class GameObject;

class ShadowMapUpdateListener : public Component {
public:
	ShadowMapUpdateListener();
	ShadowMapUpdateListener(Object* object_);
	~ShadowMapUpdateListener();

	static inline ComponentIdType GetTypeId()  { return componentTypeId; }

	// @Override
	virtual void HandleMessage(MessageChunk messageChunk);

private:
	void init();
	void destroy();

	static ComponentIdType componentTypeId;
};


#endif // SHADOW_MAP_UPDATE_LISTENER_H
