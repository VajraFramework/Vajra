//
//  AiKnowledge.h
//  Created by Matt Kaufmann on 01/17/14.
//

#ifndef AIKNOWLEDGE_H
#define AIKNOWLEDGE_H

#include "Vajra/Common/Components/Component.h"

#include <map>

struct UnitInformation {
	float Awareness;
	// TODO [Implement] Fill this in.
};

//[[COMPONENT]]//
class AiKnowledge : public Component {
public:
	AiKnowledge();
	AiKnowledge(Object* object_);
	~AiKnowledge();

	static inline ComponentIdType GetTypeId() { return AiKnowledge::componentTypeId; }

private:
	void init();
	void destroy();

	void populateInformationMap();

	std::map<ObjectIdType, UnitInformation*> myKnowledge;

	static ComponentIdType componentTypeId;
};

#endif // AIKNOWLEDGE_H
