//
//  GameMessageTypes.h
//  Created by Matt Kaufmann on 01/05/13.
//

#ifndef GAMEMESSAGETYPES_H
#define GAMEMESSAGETYPES_H

#include "Vajra/Common/Messages/Declarations.h"

enum GameMessageType {
	MESSAGE_TYPE_GRID_CELL_CHANGED = NUM_ENGINE_MESSAGE_TYPES,
	MESSAGE_TYPE_CAMERA_MODE_CHANGED = NUM_ENGINE_MESSAGE_TYPES+1,
};

#endif // GAMEMESSAGETYPES_H
