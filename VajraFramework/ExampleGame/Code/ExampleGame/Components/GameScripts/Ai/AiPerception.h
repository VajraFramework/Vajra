//
//  AiPerception.h
//  Created by Matt Kaufmann on 01/17/14.
//

#ifndef AIPERCEPTION_H
#define AIPERCEPTION_H

#include "ExampleGame/Components/GameScripts/Ai/AiKnowledge.h"
#include "Vajra/Common/Components/Component.h"

class AiPerception : public Component {
public:
	AiPerception();
	AiPerception(Object* object_);
	~AiPerception();

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

	static ComponentIdType componentTypeId;
};

#endif // AIPERCEPTION_H
