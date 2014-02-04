//
//  AiKnowledge.h
//  Created by Matt Kaufmann on 01/17/14.
//

#ifndef AIKNOWLEDGE_H
#define AIKNOWLEDGE_H

#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"
#include "Libraries/glm/glm.hpp"
#include "Vajra/Common/Components/Component.h"

#include <map>

#define AI_AWARENESS_THRESHOLD_LOCATION  0.25f
#define AI_AWARENESS_THRESHOLD_UNIT_TYPE 0.25f

struct UnitInformation {
	float Awareness;
	glm::vec3 Location;
	UnitType Type;

	UnitInformation() {
		this->Awareness = 0.0f;
		this->Location  = ZERO_VEC3;
		this->Type      = UNIT_TYPE_UNKNOWN;
	}
};

//[[COMPONENT]]//
class AiKnowledge : public Component {
public:
	AiKnowledge();
	AiKnowledge(Object* object_);
	~AiKnowledge();

	static inline ComponentIdType GetTypeId() { return AiKnowledge::componentTypeId; }

	void LearnAboutObject(ObjectIdType objId, float awareness);

private:
	void init();
	void destroy();

	void populateInformationMap();

	std::map<ObjectIdType, UnitInformation*> myKnowledge;

	static ComponentIdType componentTypeId;
};

#endif // AIKNOWLEDGE_H
