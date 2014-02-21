//
//  TriggerTransformation.h
//  Created by Matt Kaufmann on 02/16/14.
//

#ifndef TRIGGERTRANSFORMATION_H
#define TRIGGERTRANSFORMATION_H

#include "ExampleGame/Components/Triggers/Triggerable.h"
#include "Libraries/glm/glm.hpp"

//[[COMPONENT]]//
class TriggerTransformation : public Triggerable {
public:
	TriggerTransformation();
	TriggerTransformation(Object* object_);
	virtual ~TriggerTransformation();

	static inline ComponentIdType GetTypeId() { return Triggerable::GetTypeId(); }

	//[[PROPERTY]]//
	virtual void SetTriggerType(std::string typeStr);
	//[[PROPERTY]]//
	void SetTranslation(float x, float y, float z);
	void SetTranslation(glm::vec3 vec);
	//[[PROPERTY]]//
	void SetRotationDegrees(float angleDegrees, float axisX, float axisY, float axisZ);
	void SetRotationDegrees(float angleDegrees, glm::vec3 axis);
	//[[PROPERTY]]//
	void SetRotationRadians(float angleRadians, float axisX, float axisY, float axisZ);
	void SetRotationRadians(float angleRadians, glm::vec3 axis);
	//[[PROPERTY]]//
	void SetScaling(float x, float y, float z);
	void SetScaling(glm::vec3 vec);
	//[[PROPERTY]]//
	inline void SetTransitTime(float seconds);
	//[[PROPERTY]]//
	inline void SetTransformedState(bool transformed);  // Only call this function from XML to set the initial state.

	virtual void HandleMessage(MessageChunk messageChunk);

	//[[PROPERTY]]//
	virtual void SubscribeToMySwitch();      // Subscribe to the switch on the same object, can be used in XML prefabs
	//[[PROPERTY]]//
	virtual void SubscribeToParentSwitch();  // Subscribe to the switch on the parent object, can be used in XML prefabs

protected:
	virtual void onSwitchToggled(bool /*switchState*/);

private:
	void init();
	void destroy();

	void startTransformation(bool transformed);

	void startTranslation(bool transformed);
	void startRotation(bool transformed);
	void startScaling(bool transformed);

	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scaling;

	float transitTime;
};

void TriggerTransformation::SetTransitTime(float seconds)         { this->transitTime = seconds;       }
void TriggerTransformation::SetTransformedState(bool transformed) { this->SetToggleState(transformed); }

#endif // TRIGGERTRANSFORMATION_H
