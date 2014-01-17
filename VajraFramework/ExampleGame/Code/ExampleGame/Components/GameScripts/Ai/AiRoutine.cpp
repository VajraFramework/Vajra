//
//  AiRoutine.h
//  Created by Matt Kaufmann on 01/15/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Ai/AiRoutine.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Vajra/Utilities/StringUtilities.h"

#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"

#include "Libraries/glm/gtx/quaternion.hpp"

AiCommandType ConvertStringToAiCommand(std::string cmd) {
	if (cmd == "START") {
		return AI_COMMAND_START;
	}
	else if (cmd == "WALK") {
		return AI_COMMAND_WALK;
	}
	else if (cmd == "LOOK") {
		return AI_COMMAND_LOOK;
	}
	else if (cmd == "WAIT") {
		return AI_COMMAND_WAIT;
	}

	return AI_COMMAND_UNKNOWN;
}

ComponentIdType AiRoutine::componentTypeId = COMPONENT_TYPE_ID_AI_ROUTINE;

AiRoutine::AiRoutine() : Component() {
	this->init();
}

AiRoutine::AiRoutine(Object* object_) : Component(object_) {
	this->init();
}

AiRoutine::~AiRoutine() {
	this->destroy();
}

void AiRoutine::init() {

}

void AiRoutine::destroy() {

}

void AiRoutine::parseTaskStrings() {
	// Clear the current list of tasks.
	markers.clear();

	// Parse list of commands.
	for (unsigned int i = 0; i < this->taskStrings.size(); ++i) {
		std::string cmdStr, argStr;
		int firstChar = 0;
		int lastChar = this->taskStrings[i].find('_');
		cmdStr = this->taskStrings[i].substr(firstChar, lastChar);
		AiCommandType cmd = ConvertStringToAiCommand(cmdStr);
		firstChar = lastChar + 1;
		argStr = this->taskStrings[i].substr(firstChar);

		switch (cmd) {
			case AI_COMMAND_START:
				this->parseStartCommand(argStr);
				break;

			case AI_COMMAND_WALK:
				this->parseWalkCommand(argStr);
				break;

			case AI_COMMAND_LOOK:
				this->parseLookCommand(argStr);
				break;

			case AI_COMMAND_WAIT:
				this->parseWaitCommand(argStr);
				break;

			default:
				break;
		}
	}

	this->currentMarker = this->markers.size() - 1;
	this->resumeSchedule();
}

void AiRoutine::parseStartCommand(std::string args) {
	int start = 0;
	int end = args.find('_');
	std::string str = args.substr(start, end);
	int xPos = StringUtilities::ConvertStringToInt(str);

	start = end + 1;
	end = args.find('_', start);
	str = args.substr(start, end);
	int zPos = StringUtilities::ConvertStringToInt(str);

	start = end + 1;
	end = args.find('_', start);
	str = args.substr(start, end);
	int xLook = StringUtilities::ConvertStringToInt(str);

	start = end + 1;
	end = args.find('_', start);
	str = args.substr(start, end);
	int zLook = StringUtilities::ConvertStringToInt(str);

	AiMarker mark;
	mark.ClockTick = 0.0f;
	mark.Position = SINGLETONS->GetGridManager()->GetCell(xPos, zPos)->center;

	glm::vec3 forward = SINGLETONS->GetGridManager()->GetCell(xLook, zLook)->center - mark.Position;
	forward = glm::normalize(forward);

	mark.Orientation = LookRotation(forward);
	this->markers.push_back(mark);

	this->GetObject()->GetComponent<Transform>()->SetOrientation(mark.Orientation);
}

void AiRoutine::parseWalkCommand(std::string args) {
	int nMarks = this->markers.size();
	AiMarker lastMark = this->markers[nMarks - 1];

	int start = 0;
	int end = args.find('_');
	std::string str = args.substr(start, end);
	int x = StringUtilities::ConvertStringToInt(str);

	start = end + 1;
	end = args.find('_', start);
	str = args.substr(start, end);
	int z = StringUtilities::ConvertStringToInt(str);

	AiMarker mark;
	mark.Position = SINGLETONS->GetGridManager()->GetCell(x, z)->center;

	float dx = mark.Position.x - lastMark.Position.x;
	float dz = mark.Position.z - lastMark.Position.z;
	float dist = sqrt(dx * dx + dz * dz);
	mark.ClockTick = lastMark.ClockTick + dist;

	glm::vec3 forward = mark.Position - lastMark.Position;
	forward = glm::normalize(forward);

	mark.Orientation = LookRotation(forward);
	this->markers.push_back(mark);
}

void AiRoutine::parseLookCommand(std::string args) {
	int nMarks = this->markers.size();
	AiMarker lastMark = this->markers[nMarks - 1];

	int start = 0;
	int end = args.find('_');
	std::string str = args.substr(start, end);
	int x = StringUtilities::ConvertStringToInt(str);

	start = end + 1;
	end = args.find('_', start);
	str = args.substr(start, end);
	int z = StringUtilities::ConvertStringToInt(str);

	AiMarker mark;
	mark.Position = lastMark.Position;

	glm::vec3 forward = SINGLETONS->GetGridManager()->GetCell(x, z)->center - mark.Position;
	forward = glm::normalize(forward);

	mark.Orientation = LookRotation(forward);
	float angle = glm::angle(glm::inverse(mark.Orientation) * lastMark.Orientation);
	mark.ClockTick = lastMark.ClockTick + (angle / 90.0f);
	this->markers.push_back(mark);
}

void AiRoutine::parseWaitCommand(std::string args) {
	int nMarks = this->markers.size();
	AiMarker lastMark = this->markers[nMarks - 1];

	int start = 0;
	int end = args.find('_');
	std::string str = args.substr(start, end);
	float t = StringUtilities::ConvertStringToFloat(str);

	AiMarker mark;
	mark.ClockTick = lastMark.ClockTick + t;
	mark.Position = lastMark.Position;
	mark.Orientation = lastMark.Orientation;
	this->markers.push_back(mark);
}

void AiRoutine::resumeSchedule() {

}
