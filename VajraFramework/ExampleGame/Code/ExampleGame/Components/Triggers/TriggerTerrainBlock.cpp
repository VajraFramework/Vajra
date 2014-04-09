//
//  TriggerTerrainBlock.cpp
//  Created by Matt Kaufmann on 02/16/14.
//

#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Triggers/TriggerTerrainBlock.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"

TriggerTerrainBlock::TriggerTerrainBlock() : Triggerable() {
	this->init();
}

TriggerTerrainBlock::TriggerTerrainBlock(Object* object_) : Triggerable(object_) {
	this->init();
}

TriggerTerrainBlock::~TriggerTerrainBlock() {
	this->destroy();
}

void TriggerTerrainBlock::init() {
	this->changeVisibility  = true;
	this->changeWalkability = true;
	this->isPassable        = false;
}

void TriggerTerrainBlock::destroy() {

}

void TriggerTerrainBlock::SetTriggerType(std::string typeStr) {
	Triggerable::SetTriggerType(typeStr);
}

void TriggerTerrainBlock::HandleMessage(MessageChunk messageChunk) {
	Triggerable::HandleMessage(messageChunk);
}

void TriggerTerrainBlock::SubscribeToMySwitch() {
	Triggerable::SubscribeToMySwitch();
}

void TriggerTerrainBlock::SubscribeToParentSwitch() {
	Triggerable::SubscribeToParentSwitch();
}

void TriggerTerrainBlock::onSwitchToggled(bool /*switchState*/) {
	this->startTerrainChange();
	this->isPassable = !this->isPassable;
}

void TriggerTerrainBlock::onSwitchActivated() {

}

void TriggerTerrainBlock::onSwitchDeactivated() {

}

void TriggerTerrainBlock::startTerrainChange() {
	// Get the grid zone component on this object.
	GridZone* zone = this->GetObject()->GetComponent<GridZone>();
	ASSERT(zone != nullptr, "Object %d with TriggerElevationChange component also has GridZone component", this->GetObject()->GetId());

	if (zone != nullptr) {
		int west, east, south, north;
		zone->GetZoneBounds(west, east, south, north);

		GameGrid* grid = SINGLETONS->GetGridManager()->GetGrid();

		int elevation = grid->GetElevationFromWorldY(this->GetObject()->GetComponent<Transform>()->GetPositionWorld().y);
		for (int x = west; x <= east; ++x) {
			for (int z = south; z <= north; ++z) {
				if (this->changeWalkability) {
					grid->SetCellPassableAtElevation(x, z, elevation, this->isPassable);
				}
				if (this->changeVisibility) {
					grid->SetCellVisibleAtElevation(x, z, elevation, this->isPassable);
				}
			}
		}
	}

	// Navigators need to update their pathfinding
	MessageChunk gridNavMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	gridNavMessage->SetMessageType(MESSAGE_TYPE_GRID_NAVIGATION_REFRESH);
	ENGINE->GetMessageHub()->SendMulticastMessage(gridNavMessage, SINGLETONS->GetGridManagerObject()->GetId());
}
