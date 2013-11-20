#ifndef TWEEN_CALLBACK_COMPONENT_H
#define TWEEN_CALLBACK_COMPONENT_H

#include "Vajra/Common/Components/Component.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Utilities/MathUtilities.h"

// Forward Declarations:
class Object;

class TweenCallbackComponent : public Component {
public:
	TweenCallbackComponent();
	TweenCallbackComponent(Object* object_);
	~TweenCallbackComponent();

	static inline unsigned int GetTypeId() { return componentTypeId; }

	// @Override
	virtual void HandleMessage(Message* message);

private:
	void init();
	void destroy();

	// Utility Functions:
	void handleCallbacksOnAnimationEnd(Message* message);

	static unsigned int componentTypeId;
};

#endif // TWEEN_CALLBACK_COMPONENT_H
