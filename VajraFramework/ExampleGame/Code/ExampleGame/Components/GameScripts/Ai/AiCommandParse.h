//
//  AiCommandParse.h
//  Created by Matt Kaufmann on 01/25/14.
//

#ifndef AICOMMANDPARSE_H
#define AICOMMANDPARSE_H

#include "Vajra/Utilities/MathUtilities.h"

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

struct AiMarker {
	float WaitTime;
	glm::vec3 Position;
	glm::quat Orientation;
};

AiCommandType ConvertStringToAiCommand(std::string cmd);
DirectionType ConvertStringToDirection(std::string cmd);
void AddDirectionToCellCoordinates(int& x, int& z, DirectionType dir, int distance);

void ParseAiCommands(std::vector<std::string>& commands, std::vector<AiMarker>& markers);
void ParseStartCommand(std::vector<std::string>& args, std::vector<AiMarker>& markers);
void ParseWalkCommand(std::vector<std::string>& args, std::vector<AiMarker>& markers);
void ParseLookCommand(std::vector<std::string>& args, std::vector<AiMarker>& markers);
void ParseWaitCommand(std::vector<std::string>& args, std::vector<AiMarker>& markers);

#endif // AICOMMANDPARSE_H
