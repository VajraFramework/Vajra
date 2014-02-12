//
//  AiRoutine.h
//  Created by Matt Kaufmann on 01/15/14.
//

#ifndef AIROUTINE_H
#define AIROUTINE_H

#include "ExampleGame/Components/GameScripts/Ai/AiCommandParse.h"
#include "Vajra/Common/Components/Component.h"

#include <string>
#include <vector>

//[[COMPONENT]]//
class AiRoutine : public Component {
public:
	AiRoutine();
	AiRoutine(Object* object_);
	~AiRoutine();

	static inline ComponentIdType GetTypeId() { return AiRoutine::componentTypeId; }

	void SetBehavior(std::vector<std::string>& commands);

	void Follow();
	void ResumeSchedule();

private:
	void init();
	void destroy();

	enum AiMarkerType {
		AI_MARKER_WALK,
		AI_MARKER_LOOK,
		AI_MARKER_WAIT,
	};

	void nextMarker();

	std::vector<std::string> taskStrings;
	std::vector<AiMarker> markers;
	int currentMarker;
	float waitTimer;
	float maxTick;
	AiMarkerType currentMarkerType;

	static ComponentIdType componentTypeId;

	friend class LevelLoader;
};

#endif // AIROUTINE_H
