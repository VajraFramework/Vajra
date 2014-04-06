//
//  PushPillar.cpp
//  Created by Matt Kaufmann on 03/31/14.
//

#include "ExampleGame/Components/GameScripts/Units/Assassin.h"
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
	this->unitType = UNIT_TYPE_PILLAR;
	this->gridNavRef->SetMaxNavigableUnitType(UNIT_TYPE_NONE);
	this->gridNavRef->SetMovementSpeed(2.0f);

	this->isSliding = false;
	this->slideX = 0;
	this->slideZ = 0;
	this->riderId = OBJECT_ID_INVALID;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_ENTERED             , this->GetTypeId(), false);
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
				this->slide();
			}
			break;

		case MESSAGE_TYPE_GRID_ZONE_ENTERED:
			if (this->isSliding && (messageChunk->GetSenderId() != this->gameObjectRef->GetId())) {
				this->childUnitOnTop();
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

bool PushPillar::CanBeKilledBy(ObjectIdType /*id*/, glm::vec3 /*source*/) {
	/*
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
	*/
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
			//if (this->CanBeKilledBy(id, source)) {
			GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
			ASSERT(gObj != nullptr, "GameObject exists with id %d", id);
			if (gObj != nullptr) {
				BaseUnit* unit = gObj->GetComponent<BaseUnit>();
				if (unit != nullptr) {
					// Pillars can only be pushed by the assassin.
					if (unit->GetUnitType() == UNIT_TYPE_ASSASSIN) {
						// Move it!
						this->startSliding(this->gameObjectRef->GetTransform()->GetPositionWorld() - source);
					}
				}
			}
		}
	}
}

void PushPillar::Kill() {

}

void PushPillar::startSliding(glm::vec3 direction) {
	// Convert the direction vector to one of the cardinal directions (N/S/E/W)
	if      (direction.x >  ROUNDING_ERROR) { this->slideX =  1; }
	else if (direction.x < -ROUNDING_ERROR) { this->slideX = -1; }
	else                                    { this->slideX =  0; }
	if      (direction.z >  ROUNDING_ERROR) { this->slideZ =  1; }
	else if (direction.z < -ROUNDING_ERROR) { this->slideZ = -1; }
	else                                    { this->slideZ =  0; }

	if ((this->slideX != 0) && (this->slideZ != 0)) {
		// Pillars can't slide diagonally. If either X or Z greatly outweighs the other, clamp to that direction. Otherwise, don't move.
		float ratio = direction.x * this->slideX * this->slideZ / direction.z;
		if (ratio < 2.0f) { this->slideX = 0; }
		if (ratio > 0.5f) { this->slideZ = 0; }
	}

	if ((this->slideX != 0) || (this->slideZ != 0)) {
		this->isSliding = true;
		this->childUnitOnTop();
		this->slide();
	}
}

void PushPillar::slide() {
	GridCell* myCell = this->gridNavRef->GetCurrentCell();
	if (myCell != nullptr) {
		GridCell* targetCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(myCell->x + this->slideX, myCell->z - this->slideZ);
		if (!this->gridNavRef->SetDestination(targetCell)) {
			this->stopSliding();
		}
	}
	else {
		this->stopSliding();
	}
}

void PushPillar::stopSliding() {
	this->isSliding = false;
	this->unchildUnitOnTop();
}

void PushPillar::childUnitOnTop() {
	GridCell* cell = this->gridNavRef->GetCurrentCell();
	if (cell != nullptr) {
		int elevation = this->gridNavRef->GetElevation() + 1;
		this->riderId = cell->GetOccupantIdAtElevation(elevation);
		if (this->riderId != OBJECT_ID_INVALID) {
			GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(riderId);
			if (gObj != nullptr) {
				this->gameObjectRef->AddChild_maintainTransform(this->riderId);

				// If it's a unit that's moving, stop it:
				GridNavigator* gNav = gObj->GetComponent<GridNavigator>();
				if (gNav != nullptr) {
					if(gObj->GetComponent<BaseUnit>()) {
						if(gObj->GetComponent<BaseUnit>()->GetUnitType() == UnitType::UNIT_TYPE_ASSASSIN) {
							gObj->GetComponent<Assassin>()->cancelSpecial();
						}
					}

					gNav->HaltMovement();
					gNav->DisableNavigation();
				}
			}
		}
	}
}

void PushPillar::unchildUnitOnTop() {
	if (this->riderId != OBJECT_ID_INVALID) {
		GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(this->riderId);
		ASSERT(gObj != nullptr, "Object exists with id %d", this->riderId);
		if (gObj != nullptr) {
			gObj->GetParentSceneGraph()->GetRootGameObject()->AddChild_maintainTransform(this->riderId);

			// Re-enable the navigator.
			GridNavigator* gNav = gObj->GetComponent<GridNavigator>();
			if (gNav != nullptr) {
				GridCell* cell = this->gridNavRef->GetCurrentCell();
				if (cell != nullptr) {
					int elevation = this->gridNavRef->GetElevation() + 1;
					gNav->SetGridPosition(cell, elevation);
				}

				gNav->EnableNavigation();
			}
		}
		this->riderId = OBJECT_ID_INVALID;
	}
}

void PushPillar::onZoneEnteredCell(int gridX, int gridZ) {
	// When the pillar enters a cell, that cell becomes passable above the pillar
	int elevation = this->gridNavRef->GetElevation();
	SINGLETONS->GetGridManager()->GetGrid()->SetCellPassableAtElevation(gridX, gridZ, elevation, false);
	if (elevation < (NUM_ELEVATIONS - 1)) {
		SINGLETONS->GetGridManager()->GetGrid()->SetCellPassableAtElevation(gridX, gridZ, elevation + 1, true);
	}
}

void PushPillar::onZoneExitedCell(int gridX, int gridZ) {
	// When the pillar exits a cell, that cell becomes impassable at the higher elevation
	int elevation = this->gridNavRef->GetElevation();
	SINGLETONS->GetGridManager()->GetGrid()->SetCellPassableAtElevation(gridX, gridZ, elevation, true);
	if (elevation < (NUM_ELEVATIONS - 1)) {
		SINGLETONS->GetGridManager()->GetGrid()->SetCellPassableAtElevation(gridX, gridZ, elevation + 1, false);
	}
}
