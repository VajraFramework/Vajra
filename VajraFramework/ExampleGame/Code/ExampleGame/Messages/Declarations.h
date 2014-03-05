//
//  GameMessageTypes.h
//  Created by Matt Kaufmann on 01/05/13.
//

#ifndef GAMEMESSAGETYPES_H
#define GAMEMESSAGETYPES_H

#include "Vajra/Common/Messages/Declarations.h"

enum GameMessageType {
	MESSAGE_TYPE_GRID_CELL_CHANGED = NUM_ENGINE_MESSAGE_TYPES,
	MESSAGE_TYPE_CAMERA_MODE_CHANGED,
	MESSAGE_TYPE_GRID_UNIT_COLLISION,
	MESSAGE_TYPE_GRID_ZONE_ENTERED,
	MESSAGE_TYPE_GRID_ZONE_EXITED,
	MESSAGE_TYPE_GRID_ZONE_ENTERED_CELL,
	MESSAGE_TYPE_GRID_ZONE_EXITED_CELL,
	MESSAGE_TYPE_GRID_ROOM_ENTERED,
	MESSAGE_TYPE_GRID_ROOM_EXITED,
	MESSAGE_TYPE_SELECTED_UNIT_CHANGED,
	MESSAGE_TYPE_GRID_NAVIGATION_REFRESH,

	// Levels
	MESSAGE_TYPE_LEVEL_LOADED,
	MESSAGE_TYPE_LEVEL_UNLOADED,
	MESSAGE_TYPE_LOAD_LEVEL,
	MESSAGE_TYPE_ON_END_CONDITIONS_MET,

	// Tutorial
	MESSAGE_TYPE_CREATED_TUTORIAL,
	// Navigation Messages
	MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION,

	// Unit Messages
	MESSAGE_TYPE_UNIT_KILLED,
	MESSAGE_TYPE_UNIT_ACTION_STATE_CHANGED,
	MESSAGE_TYPE_UNIT_SPECIAL_HIT,

	// AI Messages
	MESSAGE_TYPE_AI_SIGHTED_PLAYER,
	MESSAGE_TYPE_AI_LOST_SIGHT_OF_PLAYER,

	// Switch Messages
	MESSAGE_TYPE_SWITCH_ACTIVATED,
	MESSAGE_TYPE_SWITCH_DEACTIVATED,

	// Multiplex Messages
	MESSAGE_TYPE_MULTIPLEX_ACTIVATED,
	MESSAGE_TYPE_MULTIPLEX_DEACTIVATED,
};

#endif // GAMEMESSAGETYPES_H
