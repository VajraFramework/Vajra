//
//  AiRoutine.h
//  Created by Matt Kaufmann on 01/15/14.
//

#ifndef AIROUTINE_H
#define AIROUTINE_H

#include "Vajra/Common/Components/Component.h"

#include <string>
#include <vector>

enum AiCommandType {
	AI_COMMAND_UNKNOWN,
	//
	AI_COMMAND_START,
	AI_COMMAND_WALK,
	AI_COMMAND_LOOK,
	AI_COMMAND_WAIT,
};

AiCommandType ConvertStringToAiCommand(std::string cmd);

//[[COMPONENT]]//
class AiRoutine : public Component {
public:
	AiRoutine();
	AiRoutine(Object* object_);
	~AiRoutine();

	void Follow();

private:
	void init();
	void destroy();

	struct AiMarker {
		float ClockTick;
		glm::vec3 Position;
		glm::quat Orientation;
	};

	enum AiMarkerType {
		AI_MARKER_WALK,
		AI_MARKER_LOOK,
		AI_MARKER_WAIT,
	};

	void parseTaskStrings();
	void parseStartCommand(std::string args);
	void parseWalkCommand(std::string args);
	void parseLookCommand(std::string args);
	void parseWaitCommand(std::string args);

	void nextMarker();
	void resumeSchedule();

	std::vector<std::string> taskStrings;
	std::vector<AiMarker> markers;
	int currentMarker;
	float currentTick;
	float maxTick;
	AiMarkerType currentMarkerType;

	static ComponentIdType componentTypeId;

	friend class LevelManager;
};

#endif // AIROUTINE_H
