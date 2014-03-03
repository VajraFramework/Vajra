//
//  UnitDeclarations.cpp
//  Created by Matt Kaufmann on 02/20/14.
//

#include "ExampleGame/Components/GameScripts/Units/UnitDeclarations.h"

UnitType ConvertStringToUnitType(std::string typeStr) {
	if (typeStr == "Assassin") {
		return UNIT_TYPE_ASSASSIN;
	}
	if (typeStr == "Thief") {
		return UNIT_TYPE_THIEF;
	}
	if (typeStr == "Guard") {
		return UNIT_TYPE_GUARD;
	}
	if (typeStr == "Captain") {
		return UNIT_TYPE_CAPTAIN;
	}
	if (typeStr == "Obstacle") {
		return UNIT_TYPE_OBSTACLE;
	}
	return UNIT_TYPE_UNKNOWN;
}
