//
//  AiPerception.h
//  Created by Matt Kaufmann on 01/17/14.
//

#ifndef AIPERCEPTION_H
#define AIPERCEPTION_H

#include "ExampleGame/Components/GameScripts/Ai/AiKnowledge.h"
#include "Vajra/Common/Components/Component.h"

class BaseUnit;

//[[COMPONENT]]//
class AiPerception : public Component {
public:
	AiPerception();
	AiPerception(Object* object_);
	~AiPerception();

	static inline ComponentIdType GetTypeId() { return AiPerception::componentTypeId; }

	void Activate();

	//[[PROPERTY]]//
	inline void SetVisionRange(float range);
	//[[PROPERTY]]//
	inline void SetFieldOfVision(float angle);
	//[[PROPERTY]]//
	inline void SetFieldOfVisionDegrees(float angle);
	//[[PROPERTY]]//
	inline void SetVisionAcuity(float acuity);

protected:
	virtual void start();
	virtual void update();

private:
	void init();
	void destroy();

	void gatherInformation();
	void gatherInformationAboutObject(ObjectIdType objId);

	float calculateAwarenessOfObject(ObjectIdType objId);
	float calculateVisualAwareness(ObjectIdType objId);
	float calculateAudioAwareness(ObjectIdType objId);

	AiKnowledge* knowledge;
	BaseUnit* unit;

	float visionRange;    // Maximum distance the AI can see
	float fieldOfVision;  // Angular range of the AI's vision
	float visionAcuity;   // Strength of the AI's vision [0..1]

	static ComponentIdType componentTypeId;
};

void AiPerception::SetVisionRange(float range)           { this->visionRange = range; }
void AiPerception::SetFieldOfVision(float angle)         { this->fieldOfVision = angle; }
void AiPerception::SetFieldOfVisionDegrees(float angle)  { this->fieldOfVision = angle inRadians; }
void AiPerception::SetVisionAcuity(float acuity)         { this->visionAcuity = acuity; }

#endif // AIPERCEPTION_H
