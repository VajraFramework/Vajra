//
//  AiRoutine.h
//  Created by Matt Kaufmann on 01/15/14.
//

#include "ExampleGame/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame/Components/GameScripts/Ai/AiRoutine.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Libraries/glm/gtx/quaternion.hpp"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Timer/Timer.h"
#include "Vajra/Utilities/StringUtilities.h"

#define MIN_DISTANCE 0.0001f
#define MIN_ANGLE    0.0001f

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

void AiRoutine::Follow() {
	GridNavigator* gNav = this->GetObject()->GetComponent<GridNavigator>();

	if (this->currentMarkerType == AI_MARKER_WALK) {
		if (!gNav->IsTraveling()) {
			this->nextMarker();
		}
	}
	else if (this->currentMarkerType == AI_MARKER_LOOK) {
		if (!gNav->IsTurning()) {
			this->nextMarker();
		}
	}
	else if (this->currentMarkerType == AI_MARKER_WAIT) {
		this->currentTick += ENGINE->GetTimer()->GetDeltaFrameTime();
		if (this->currentTick >= this->markers[this->currentMarker].ClockTick) {
			this->nextMarker();
		}
	}
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

	this->currentTick = 0.0f;
	this->maxTick = this->markers[this->markers.size()].ClockTick;
	this->currentMarker = 0;
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

	mark.Orientation = QuaternionFromLookVectors(forward);
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

	mark.Orientation = QuaternionFromLookVectors(forward);
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

	mark.Orientation = QuaternionFromLookVectors(forward);
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

void AiRoutine::nextMarker() {
	this->currentTick = this->markers[this->currentMarker].ClockTick;
	if (this->currentTick >= this->maxTick) {
		this->currentTick -= this->maxTick;
	}
	this->currentMarker = (this->currentMarker + 1) % this->markers.size();

	resumeSchedule();
}

void AiRoutine::resumeSchedule() {
	Transform* trans = this->GetObject()->GetComponent<Transform>();

	AiMarker prevMark;
	prevMark.ClockTick = this->currentTick;
	prevMark.Position = trans->GetPositionWorld();
	prevMark.Orientation = trans->GetOrientationWorld();
	AiMarker newMark = this->markers[this->currentMarker];

	float dist = glm::distance(prevMark.Position, newMark.Position);
	float angle = glm::angle(glm::inverse(prevMark.Orientation) * newMark.Orientation);
	GridNavigator* gNav = this->GetObject()->GetComponent<GridNavigator>();
	if (dist > MIN_DISTANCE) {
		this->currentMarkerType = AI_MARKER_WALK;
		gNav->SetDestination(newMark.Position);
	}
	else if (angle > MIN_ANGLE) {
		this->currentMarkerType = AI_MARKER_LOOK;
		gNav->SetLookTarget(newMark.Orientation);
	}
	else {
		this->currentMarkerType = AI_MARKER_WAIT;
	}
}
