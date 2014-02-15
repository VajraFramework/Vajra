//
//  Triggerable.h
//  Created by Matt Kaufmann on 02/11/14.
//

#ifndef TRIGGERABLE_H
#define TRIGGERABLE_H

#include "Vajra/Common/Components/Component.h"

#include <list>

class Triggerable : public Component {
public:
	Triggerable();
	Triggerable(Object* object_);
	virtual ~Triggerable();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	virtual void HandleMessage(MessageChunk messageChunk);

	void SubscribeToSwitchObject(ObjectIdType switchId);   // This must be called before this component can trigger
	void UnsubscribeToSwitchObject(ObjectIdType switchId);

protected:
	virtual void onSwitchToggled(bool /*switchState*/) { }
	virtual void onSwitchActivated() { }
	virtual void onSwitchDeactivated() { }

private:
	void init();
	void destroy();

	std::list<ObjectIdType> subscriptions;

	static ComponentIdType componentTypeId;
};

#endif // TRIGGERABLE_H
