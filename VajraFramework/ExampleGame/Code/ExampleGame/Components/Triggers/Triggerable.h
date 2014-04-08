//
//  Triggerable.h
//  Created by Matt Kaufmann on 02/11/14.
//

#ifndef TRIGGERABLE_H
#define TRIGGERABLE_H

#include "Vajra/Common/Components/Component.h"

#include <list>
#include <string>

enum TriggerType {
	TRIGGER_TYPE_ALL,      // The trigger will activate only once all switches are on at once
	TRIGGER_TYPE_ANY,      // The trigger will activate if any switches are turned on
	TRIGGER_TYPE_INVALID,
};

TriggerType ConvertStringToTriggerType(std::string str);

// Forward Declarations:
class GameObject;

//[[COMPONENT]]//
class Triggerable : public Component {
public:
	Triggerable();
	Triggerable(Object* object_);
	virtual ~Triggerable();

	static inline ComponentIdType GetTypeId() { return componentTypeId; }

	//[[PROPERTY]]//
	virtual void SetTriggerType(std::string typeStr);
	inline void SetTriggerType(TriggerType st) { this->type = st; }
	//[[PROPERTY]]//
	virtual void SetToggleState(bool toggle);
	//[[PROPERTY]]//
	virtual void SetActivationAudio(std::string audioStr);
	//[[PROPERTY]]//
	virtual void SetDeactivationAudio(std::string audioStr);

	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	virtual void SubscribeToMySwitch();      // Subscribe to the switch on the same object, can be used in XML prefabs
	//[[PROPERTY]]//
	virtual void SubscribeToParentSwitch();  // Subscribe to the switch on the parent object, can be used in XML prefabs
	void SubscribeToSwitchObject(ObjectIdType switchId);   // This must be called before this component can trigger
	void UnsubscribeToSwitchObject(ObjectIdType switchId);
	void UnsubscribeToAllSwitches();

	//[[PROPERTY]]//
	void SetDecalType(std::string decalType);

	std::string GetDecalType() { return this->decalType; }

protected:
	virtual void start();

	virtual void onSwitchToggled(bool /*switchState*/) { }
	virtual void onSwitchActivated() { }
	virtual void onSwitchDeactivated() { }

	void setDecalType(std::string decalType_);

	TriggerType type;
	bool isToggled;

private:
	void init();
	void destroy();

	void subscribeInternal(ObjectIdType switchId);
	void unsubscribeInternal(ObjectIdType switchId);
	void markAsActive(ObjectIdType switchId);
	void markAsInactive(ObjectIdType switchId);

	void compareCounts(int prevActive, int prevSwitches, int currActive, int currSwitches);

	void toggleState();

	std::list<ObjectIdType> subscriptions;
	std::list<ObjectIdType> activeSwitches;

	std::string decalType;
	GameObject* decalRef;

	std::string audioOnActivate;
	std::string audioOnDeactivate;

	static ComponentIdType componentTypeId;
};

#endif // TRIGGERABLE_H
