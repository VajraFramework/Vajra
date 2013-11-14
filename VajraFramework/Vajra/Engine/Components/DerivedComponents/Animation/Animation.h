#ifndef ANIMATION_H
#define ANIMATION_H

#include "Vajra/Common/Components/Component.h"

#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtc/quaternion.hpp"
#include "Libraries/glm/gtx/quaternion.hpp"

// Forward Declarations:
class GameObject;

class Animation : public Component {
public:
	Animation();
	Animation(Object* object_);
	~Animation();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(Message* message);

private:
	void init();
	void destroy();


	static unsigned int componentTypeId;
};

#endif // ANIMATION_H
