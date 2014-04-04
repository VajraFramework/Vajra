//
//  PushPillar.cpp
//  Created by Matt Kaufmann on 03/31/14.
//

#include "ExampleGame/Components/GameScripts/Units/Obstacles/PushPillar.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Utilities/MathUtilities.h"

PushPillar::PushPillar() : BaseUnit() {
	this->init();
}

PushPillar::PushPillar(Object* object_) : BaseUnit(object_) {
	this->init();
}

PushPillar::~PushPillar() {
	this->destroy();
}

void PushPillar::init() {
	this->unitType = UNIT_TYPE_OBSTACLE;
	this->gridNavRef->SetMaxNavigableUnitType(UNIT_TYPE_NONE);
	this->gridNavRef->SetMovementSpeed(2.0f);

	isSliding = false;
	slideDir = ZERO_VEC3;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_ENTERED_CELL        , this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_EXITED_CELL         , this->GetTypeId(), false);
}

void PushPillar::destroy() {

}

void PushPillar::HandleMessage(MessageChunk messageChunk) {
	BaseUnit::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION:
			if (this->isSliding) {
				this->moveToBlockInDirection();
			}
			break;

		case MESSAGE_TYPE_GRID_ZONE_ENTERED_CELL:
			if (messageChunk->GetSenderId() == this->GetObject()->GetId()) {
				this->onZoneEnteredCell(messageChunk->messageData.iv1.x, messageChunk->messageData.iv1.z);
			}
			break;

		case MESSAGE_TYPE_GRID_ZONE_EXITED_CELL:
			if (messageChunk->GetSenderId() == this->GetObject()->GetId()) {
				this->onZoneExitedCell(messageChunk->messageData.iv1.x, messageChunk->messageData.iv1.z);
			}
			break;
	}
}

bool PushPillar::CanBeKilledBy(ObjectIdType id, glm::vec3 /*source*/) {
	GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
	ASSERT(gObj != nullptr, "GameObject exists with id %d", id);
	if (gObj != nullptr) {
		BaseUnit* unit = gObj->GetComponent<BaseUnit>();
		if (unit != nullptr) {
			// Pillars can only be pushed by the assassin.
			if (unit->GetUnitType() == UNIT_TYPE_ASSASSIN) {
				return true;
			}
		}
	}
	return false;
}

void PushPillar::start() {
	this->gridNavRef->SetTurnSpeedRadians(0.0f);
}

void PushPillar::end() {

}

void PushPillar::update() {

}

void PushPillar::onUnitSpecialHit(ObjectIdType id, int gridX, int gridZ, glm::vec3 source) {
	// First check if the attack hit my cell
	GridCell* cell = this->gridNavRef->GetCurrentCell();
	if (cell != nullptr) {
		if ((cell->x == gridX) && (cell->z == gridZ)) {
			// Check if the attacker can kill me
			if (this->CanBeKilledBy(id, source)) {
				// Move it!
				this->slideDir = this->gameObjectRef->GetTransform()->GetPositionWorld() - source;
				this->moveToBlockInDirection();
			}
		}
	}
}

void PushPillar::Kill() {

}

//void PushPillar::moveToBlockInDirection(glm::vec3 dir) {
void PushPillar::moveToBlockInDirection() {
	int xDir, zDir;
	if      (this->slideDir.x >  ROUNDING_ERROR) { xDir =  1; }
	else if (this->slideDir.x < -ROUNDING_ERROR) { xDir = -1; }
	else                                         { xDir =  0; }
	if      (this->slideDir.z >  ROUNDING_ERROR) { zDir =  1; }
	else if (this->slideDir.z < -ROUNDING_ERROR) { zDir = -1; }
	else                                         { zDir =  0; }

	if ((xDir != 0) || (zDir != 0)) {
		if ((xDir != 0) && (zDir != 0)) {
			float ratio = this->slideDir.x * xDir * zDir / this->slideDir.z;

			// If the vector points primarily in either the X or Z direction, clamp it to that direction.
			if      (ratio >= 2.0f) { zDir = 0; }
			else if (ratio <= 0.5f) { xDir = 0; }
		}

		GridCell* myCell = this->gridNavRef->GetCurrentCell();
		GridCell* targetCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(myCell->x + xDir, myCell->z - zDir);
		if (targetCell != nullptr) {
			this->isSliding = this->gridNavRef->SetDestination(targetCell);
		}
	}
}

void PushPillar::onZoneEnteredCell(int gridX, int gridZ) {
	// When the pillar enters a cell, that cell becomes passable above the pillar
	int elevation = this->gridNavRef->GetElevation();
	if (elevation < (NUM_ELEVATIONS - 1)) {
		SINGLETONS->GetGridManager()->GetGrid()->SetCellPassableAtElevation(gridX, gridZ, elevation + 1, true);
	}
}

void PushPillar::onZoneExitedCell(int gridX, int gridZ) {
	// When the pillar exits a cell, that cell becomes impassable at the higher elevation
	int elevation = this->gridNavRef->GetElevation();
	if (elevation < (NUM_ELEVATIONS - 1)) {
		SINGLETONS->GetGridManager()->GetGrid()->SetCellPassableAtElevation(gridX, gridZ, elevation + 1, false);
	}
}
