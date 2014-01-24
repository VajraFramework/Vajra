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

enum DirectionType {
	DIR_INVALID,
	//
	DIR_NORTH,
	DIR_EAST,
	DIR_SOUTH,
	DIR_WEST,
	//
	DIR_NORTHEAST,
	DIR_SOUTHEAST,
	DIR_SOUTHWEST,
	DIR_NORTHWEST,
};

AiCommandType ConvertStringToAiCommand(std::string cmd);
DirectionType ConvertStringToDirection(std::string cmd);
void AddDirectionToCellCoordinates(int& x, int& z, DirectionType dir, int distance);

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
		float WaitTime;
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
	float waitTimer;
	float maxTick;
	AiMarkerType currentMarkerType;

	static ComponentIdType componentTypeId;

	friend class LevelLoader;
};

#endif // AIROUTINE_H
