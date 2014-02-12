//
//  AiCommandParse.cpp
//  Created by Matt Kaufmann on 01/25/14.
//

#include "ExampleGame/Components/GameScripts/Ai/AiCommandParse.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Vajra/Utilities/FileUtilities.h"
#include "Vajra/Utilities/StringUtilities.h"

AiCommandType ConvertStringToAiCommand(std::string cmd) {
	if      (cmd == "START")  { return AI_COMMAND_START; }
	else if (cmd == "WALK" )  { return AI_COMMAND_WALK;  }
	else if (cmd == "LOOK" )  { return AI_COMMAND_LOOK;  }
	else if (cmd == "WAIT" )  { return AI_COMMAND_WAIT;  }
	return AI_COMMAND_UNKNOWN;
}

DirectionType ConvertStringToDirection(std::string dir) {
	if      (dir == "NORTH"    )  { return DIR_NORTH;     }
	else if (dir == "EAST"     )  { return DIR_EAST;      }
	else if (dir == "SOUTH"    )  { return DIR_SOUTH;     }
	else if (dir == "WEST"     )  { return DIR_WEST;      }
	else if (dir == "NORTHEAST")  { return DIR_NORTHEAST; }
	else if (dir == "SOUTHEAST")  { return DIR_SOUTHEAST; }
	else if (dir == "SOUTHWEST")  { return DIR_SOUTHWEST; }
	else if (dir == "NORTHWEST")  { return DIR_NORTHWEST; }
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

void ParseAiCommands(std::vector<std::string>& commands, std::vector<AiMarker>& markers) {
	for (unsigned int i = 0; i < commands.size(); ++i) {
		std::vector<std::string> commandArgs = StringUtilities::SplitStringIntoTokensOnDelimiter(commands[i], '_');

		std::string commandStr = commandArgs[0];  // Extract the actual command from the list
		commandArgs.erase(commandArgs.begin());   // Remove the command from the list, leaving only the arguments

		AiCommandType command = ConvertStringToAiCommand(commandStr);

		switch (command) {
			case AI_COMMAND_START:
				ParseStartCommand(commandArgs, markers);
				break;

			case AI_COMMAND_WALK:
				ParseWalkCommand(commandArgs, markers);
				break;

			case AI_COMMAND_LOOK:
				ParseLookCommand(commandArgs, markers);
				break;

			case AI_COMMAND_WAIT:
				ParseWaitCommand(commandArgs, markers);
				break;

			default:
				break;
		}
	}
}

void ParseStartCommand(std::vector<std::string>& args, std::vector<AiMarker>& markers) {
	// Possible argument lists are as follows:
	// - X_POS, Z_POS, LOOK_DIR
	// - X_POS, Z_POS, X_LOOK, Z_LOOK

	int xPos = StringUtilities::ConvertStringToInt(args[0]);
	int zPos = StringUtilities::ConvertStringToInt(args[1]);
	int xLook = xPos;
	int zLook = zPos;

	// Determine if the third argument is a coordinate or a direction
	DirectionType lookDir = ConvertStringToDirection(args[2]);
	if (lookDir != DIR_INVALID) {
		AddDirectionToCellCoordinates(xLook, zLook, lookDir, 1);
	}
	else {
		// The look direction is instead defined as a cell offset
		int xDiff = StringUtilities::ConvertStringToInt(args[2]);
		int zDiff = StringUtilities::ConvertStringToInt(args[3]);
		xLook += xDiff;
		zLook += zDiff;
	}

	// Create the marker representing this command
	AiMarker marker;
	marker.WaitTime = 0.0f;
	marker.Position = SINGLETONS->GetGridManager()->GetGrid()->GetCell(xPos, zPos)->center;

	glm::vec3 lookPos = SINGLETONS->GetGridManager()->GetGrid()->GetCell(xLook, zLook)->center;
	VERIFY(lookPos != marker.Position, "Valid Ai command, lookPos != marker position");
	glm::vec3 forward = lookPos - marker.Position;
	forward.y = 0.0f;
	forward = glm::normalize(forward);

	marker.Orientation = QuaternionFromLookVectors(forward);
	markers.push_back(marker);
}

void ParseWalkCommand(std::vector<std::string>& args, std::vector<AiMarker>& markers) {
	ASSERT(markers.size() > 0, "WALK command is not the first AI command");
	AiMarker prevMarker = markers[markers.size() - 1];
	// Possible argument lists are as follows:
	// - DIRECTION, DISTANCE
	// - DIRECTION, DISTANCE, DIRECTION, DISTANCE
	// - X_POS, Z_POS

	int xPos =  (int)prevMarker.Position.x;
	int zPos = -(int)prevMarker.Position.z;

	// Determine if the first argument is a coordinate or a direction
	DirectionType firstDir = ConvertStringToDirection(args[0]);
	if (firstDir != DIR_INVALID) {
		int firstDist = StringUtilities::ConvertStringToInt(args[1]);
		AddDirectionToCellCoordinates(xPos, zPos, firstDir, firstDist);

		// If there are more than two arguments, then there's another directional argument
		if (args.size() > 2) {
			DirectionType secondDir = ConvertStringToDirection(args[2]);
			int secondDist = StringUtilities::ConvertStringToInt(args[3]);
			AddDirectionToCellCoordinates(xPos, zPos, secondDir, secondDist);
		}
	}
	else {
		// The target position is defined as a cell offset
		int xDiff = StringUtilities::ConvertStringToInt(args[0]);
		int zDiff = StringUtilities::ConvertStringToInt(args[1]);
		xPos += xDiff;
		zPos += zDiff;
	}

	// Create the marker representing this command
	AiMarker marker;
	marker.WaitTime = 0.0f;
	marker.Position = SINGLETONS->GetGridManager()->GetGrid()->GetCell(xPos, zPos)->center;

	glm::vec3 forward = marker.Position - prevMarker.Position;
	forward.y = 0.0f;
	forward = glm::normalize(forward);

	marker.Orientation = QuaternionFromLookVectors(forward);
	markers.push_back(marker);
}

void ParseLookCommand(std::vector<std::string>& args, std::vector<AiMarker>& markers) {
	ASSERT(markers.size() > 0, "LOOK command is not the first AI command");
	AiMarker prevMarker = markers[markers.size() - 1];
	// Possible argument lists are as follows:
	// - DIRECTION, WAIT_TIME
	// - X_POS, Z_POS, WAIT_TIME

	int xLook =  (int)prevMarker.Position.x;
	int zLook = -(int)prevMarker.Position.z;
	unsigned int waitIndex;

	// Determine if the first argument is a coordinate or a direction
	DirectionType lookDir = ConvertStringToDirection(args[0]);
	if (lookDir != DIR_INVALID) {
		AddDirectionToCellCoordinates(xLook, zLook, lookDir, 1);
		waitIndex = 1;
	}
	else {
		// The target position is defined as a cell offset
		int xDiff = StringUtilities::ConvertStringToInt(args[0]);
		int zDiff = StringUtilities::ConvertStringToInt(args[1]);
		xLook += xDiff;
		zLook += zDiff;
		waitIndex = 2;
	}

	// Create the marker representing this command
	AiMarker marker;
	marker.WaitTime = 0.0f;
	marker.Position = prevMarker.Position;

	GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(xLook, zLook);
	VERIFY(cell != nullptr, "Got valid grid cell for location (%d, %d)", xLook, zLook);
	glm::vec3 lookPos = cell->center;
	VERIFY(lookPos != marker.Position, "Valid Ai command, lookPos != marker position");
	glm::vec3 forward = glm::normalize(lookPos - marker.Position);

	marker.Orientation = QuaternionFromLookVectors(forward);
	markers.push_back(marker);

	// Check whether there's a wait command embedded in here as well
	if (args.size() > waitIndex) {
		std::vector<std::string> waitArgs;
		waitArgs.push_back(args[waitIndex]);
		ParseWaitCommand(waitArgs, markers);
	}
}

void ParseWaitCommand(std::vector<std::string>& args, std::vector<AiMarker>& markers) {
	ASSERT(markers.size() > 0, "WAIT command is not the first AI command");
	AiMarker prevMarker = markers[markers.size() - 1];
	// Possible argument lists are as follows:
	// - WAIT_TIME

	ASSERT(args.size() > 0, "WAIT command has at least one argument");
	float wt = StringUtilities::ConvertStringToFloat(args[0]);

	// Create the marker representing this command
	AiMarker marker;
	marker.WaitTime    = wt;
	marker.Position    = prevMarker.Position;
	marker.Orientation = prevMarker.Orientation;
	markers.push_back(marker);
}
