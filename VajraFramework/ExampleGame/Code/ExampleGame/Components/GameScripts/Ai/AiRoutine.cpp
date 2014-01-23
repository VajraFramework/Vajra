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
	if      (cmd == "START")  { return AI_COMMAND_START; }
	else if (cmd == "WALK" )  { return AI_COMMAND_WALK;  }
	else if (cmd == "LOOK" )  { return AI_COMMAND_LOOK;  }
	else if (cmd == "WAIT" )  { return AI_COMMAND_WAIT;  }
	return AI_COMMAND_UNKNOWN;
}

DirectionType ConvertStringToDirection(std::string dir) {
	if      (dir == "N" )  { return DIR_NORTH;     }
	else if (dir == "E" )  { return DIR_EAST;      }
	else if (dir == "S" )  { return DIR_SOUTH;     }
	else if (dir == "W" )  { return DIR_WEST;      }
	else if (dir == "NE")  { return DIR_NORTHEAST; }
	else if (dir == "SE")  { return DIR_SOUTHEAST; }
	else if (dir == "SW")  { return DIR_SOUTHWEST; }
	else if (dir == "NW")  { return DIR_NORTHWEST; }
	return DIR_INVALID;
}

void AddDirectionToCellCoordinates(int& x, int& z, DirectionType dir, int distance) {
	switch (dir) {
		case DIR_NORTH:
			z += distance;
			break;
		case DIR_EAST:
			x += distance;
			break;
		case DIR_SOUTH:
			z -= distance;
			break;
		case DIR_WEST:
			x -= distance;
			break;
		case DIR_NORTHEAST:
			x += distance;
			z += distance;
			break;
		case DIR_SOUTHEAST:
			x += distance;
			z -= distance;
			break;
		case DIR_SOUTHWEST:
			x -= distance;
			z -= distance;
			break;
		case DIR_NORTHWEST:
			x += distance;
			z -= distance;
			break;
		default:
			break;
	}
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
	// Determine starting position
	int start = 0;
	int end = args.find('_');
	std::string str = args.substr(start, end - start);
	int xPos = StringUtilities::ConvertStringToInt(str);

	start = end + 1;
	end = args.find('_', start);
	str = args.substr(start, end - start);
	int zPos = StringUtilities::ConvertStringToInt(str);

	AiMarker mark;
	mark.ClockTick = 0.0f;
	mark.Position = SINGLETONS->GetGridManager()->GetCell(xPos, zPos)->center;

	start = end + 1;
	end = args.find('_', start);
	str = args.substr(start, end - start);

	DirectionType dir = ConvertStringToDirection(str);
	int xLook = xPos;
	int zLook = zPos;
	if (dir != DIR_INVALID) {
		// The second argument is a direction
		AddDirectionToCellCoordinates(xLook, zLook, dir, 1);
	}
	else {
		// The second argument is a cell position
		xLook = StringUtilities::ConvertStringToInt(str);

		start = end + 1;
		end = args.find('_', start);
		str = args.substr(start, end - start);
		zLook = StringUtilities::ConvertStringToInt(str);
	}
	glm::vec3 forward = SINGLETONS->GetGridManager()->GetCell(xLook, zLook)->center - mark.Position;
	forward = glm::normalize(forward);

	mark.Orientation = QuaternionFromLookVectors(forward);

	this->markers.push_back(mark);
	this->GetObject()->GetComponent<Transform>()->SetOrientation(mark.Orientation);
}

void AiRoutine::parseWalkCommand(std::string args) {
	int nMarks = this->markers.size();
	AiMarker lastMark = this->markers[nMarks - 1];

	unsigned int start = 0;
	unsigned int end = args.find('_');
	std::string str = args.substr(start, end - start);

	DirectionType dir = ConvertStringToDirection(str);
	int x =  (int)lastMark.Position.x;
	int z = -(int)lastMark.Position.z;
	if (dir != DIR_INVALID) {
		start = end + 1;
		end = args.find('_', start);
		str = args.substr(start, end - start);
		int distance = StringUtilities::ConvertStringToInt(str);
		AddDirectionToCellCoordinates(x, z, dir, distance);

		if (end != std::string::npos) {
			// We can support two different directions in one command.
			start = end + 1;
			end = args.find('_', start);
			str = args.substr(start, end - start);
			dir = ConvertStringToDirection(str);
			if (dir != DIR_INVALID) {
				start = end + 1;
				end = args.find('_', start);
				str = args.substr(start, end - start);
				distance = StringUtilities::ConvertStringToInt(str);
				AddDirectionToCellCoordinates(x, z, dir, distance);
			}
		}
	}
	else {
		x = StringUtilities::ConvertStringToInt(str);

		start = end + 1;
		end = args.find('_', start);
		str = args.substr(start, end - start);
		z = StringUtilities::ConvertStringToInt(str);
	}

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
	std::string str = args.substr(start, end - start);

	DirectionType dir = ConvertStringToDirection(str);
	int x =  (int)lastMark.Position.x;
	int z = -(int)lastMark.Position.z;
	if (dir != DIR_INVALID) {
		AddDirectionToCellCoordinates(x, z, dir, 1);
	}
	else {
		x = StringUtilities::ConvertStringToInt(str);

		start = end + 1;
		end = args.find('_', start);
		str = args.substr(start, end - start);
		z = StringUtilities::ConvertStringToInt(str);
	}

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
	std::string str = args.substr(start, end - start);
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
