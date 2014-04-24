//
//  TriggerParticles.h
//  Created by Matt Kaufmann on 04/17/14.
//

#ifndef TRIGGERPARTICLES_H
#define TRIGGERPARTICLES_H

#include "ExampleGame/Components/Triggers/Triggerable.h"
#include "Vajra/Engine/ParticleSystems/ParticleSystem.h"

//[[COMPONENT]]//
class TriggerParticles : public Triggerable {
public:
	TriggerParticles();
	TriggerParticles(Object* object_);
	virtual ~TriggerParticles();

	static inline ComponentIdType GetTypeId() { return Triggerable::GetTypeId(); }

	//[[PROPERTY]]//
	virtual void SetTriggerType(std::string typeStr);
	//[[PROPERTY]]//
	void SetLooped(bool loop);

	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	virtual void SubscribeToMySwitch();      // Subscribe to the switch on the same object, can be used in XML prefabs
	//[[PROPERTY]]//
	virtual void SubscribeToParentSwitch();  // Subscribe to the switch on the parent object, can be used in XML prefabs

protected:
	virtual void start();

	virtual void onSwitchToggled(bool switchState);

private:
	void init();
	void destroy();

	ParticleSystem* particleSystem;
	bool looped;
};

#endif // TRIGGERPARTICLES_H
