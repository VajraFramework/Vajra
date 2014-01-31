//
//  AiPerception.h
//  Created by Matt Kaufmann on 01/17/14.
//

#ifndef AIPERCEPTION_H
#define AIPERCEPTION_H

#include "ExampleGame/Components/GameScripts/Ai/AiKnowledge.h"
#include "Vajra/Common/Components/Component.h"

//[[COMPONENT]]//
class AiPerception : public Component {
public:
	AiPerception();
	AiPerception(Object* object_);
	~AiPerception();

	static inline ComponentIdType GetTypeId() { return AiPerception::componentTypeId; }

	void Activate();

	//[[PROPERTY]]//
	inline void SetVisionRange(float range)    { this->visionRange = range; }
	//[[PROPERTY]]//
	inline void SetFieldOfVision(float angle)  { this->fieldOfVision = angle; }
	//[[PROPERTY]]//
	inline void SetVisionAcuity(float acuity)  { this->visionAcuity = acuity; }

protected:
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

	float visionRange;    // Maximum distance the AI can see
	float fieldOfVision;  // Angular range of the AI's vision
	float visionAcuity;   // Strength of the AI's vision [0..1]

	static ComponentIdType componentTypeId;
};

#endif // AIPERCEPTION_H
