//
//  TransitionZone.cpp
//  Created by Matt Kaufmann on 01/31/14.
//

#include "ExampleGame/Components/GameScripts/Units/BaseUnit.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/Grid/GridZones/TransitionZone.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"

TransitionZone::TransitionZone() : GridZone() {
	this->init();
}

TransitionZone::TransitionZone(Object* object_) : GridZone(object_) {
	this->init();
}

TransitionZone::~TransitionZone() {
	this->destroy();
}

void TransitionZone::HandleMessage(MessageChunk messageChunk) {
	GridZone::HandleMessage(messageChunk);
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_GRID_ZONE_ENTERED:
			this->onUnitEnteredZone(messageChunk->GetSenderId(), SINGLETONS->GetGridManager()->GetGrid()->GetCell(messageChunk->messageData.iv1.x, messageChunk->messageData.iv1.z));
			break;
	}
}

void TransitionZone::GetFirstDestinationCoordinates(int& cellX, int& cellZ) {
	Transform* trans = this->GetObject()->GetComponent<Transform>();

	int centerX, centerZ;
	SINGLETONS->GetGridManager()->GetGrid()->GetCoordinates(centerX, centerZ, trans->GetPositionWorld());

	glm::vec3 forward = trans->GetForward();

	// Determine the rough orientation of the object.
	if (abs(forward.z) >= abs(forward.x)) {
		if (forward.z >= 0.0f) {
			cellX = centerX + this->destX1;
			cellZ = centerZ + this->destZ1;
		}
		else {
			cellX = centerX - this->destX1;
			cellZ = centerZ - this->destZ1;
		}
	}
	else {
		if (forward.x >= 0.0f) {
			cellX = centerX - this->destZ1;
			cellZ = centerZ + this->destX1;
		}
		else {
			cellX = centerX + this->destZ1;
			cellZ = centerZ - this->destX1;
		}
	}
}

void TransitionZone::GetSecondDestinationCoordinates(int& cellX, int& cellZ) {
	Transform* trans = this->GetObject()->GetComponent<Transform>();

	int centerX, centerZ;
	SINGLETONS->GetGridManager()->GetGrid()->GetCoordinates(centerX, centerZ, trans->GetPositionWorld());

	glm::vec3 forward = trans->GetForward();

	// Determine the rough orientation of the object.
	if (abs(forward.z) >= abs(forward.x)) {
		if (forward.z >= 0.0f) {
			cellX = centerX + this->destX2;
			cellZ = centerZ + this->destZ2;
		}
		else {
			cellX = centerX - this->destX2;
			cellZ = centerZ - this->destZ2;
		}
	}
	else {
		if (forward.x >= 0.0f) {
			cellX = centerX - this->destZ2;
			cellZ = centerZ + this->destX2;
		}
		else {
			cellX = centerX + this->destZ2;
			cellZ = centerZ - this->destX2;
		}
	}
}

void TransitionZone::GetBothDestinationCoordinates(int& cellX1, int& cellZ1, int& cellX2, int& cellZ2) {
	this->GetFirstDestinationCoordinates(cellX1, cellZ1);
	this->GetSecondDestinationCoordinates(cellX2, cellZ2);
}

void TransitionZone::SetZoneBounds(int xMin, int zMin, int xMax, int zMax) {
	GridZone::SetZoneBounds(xMin, zMin, xMax, zMax);
}

void TransitionZone::SetFirstDestination(int cellX, int cellZ) {
	this->destX1 = cellX;
	this->destZ1 = cellZ;
}

void TransitionZone::SetSecondDestination(int cellX, int cellZ) {
	this->destX2 = cellX;
	this->destZ2 = cellZ;
}

void TransitionZone::SetDestinations(int cellX1, int cellZ1, int cellX2, int cellZ2) {
	this->SetFirstDestination(cellX1, cellZ1);
	this->SetSecondDestination(cellX2, cellZ2);
}

void TransitionZone::init() {
	this->destX1 = 0;
	this->destZ1 = 0;
	this->destX2 = 0;
	this->destZ2 = 0;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_ENTERED, this->GetTypeId(), false);
}

void TransitionZone::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void TransitionZone::onUnitEnteredZone(ObjectIdType id, GridCell* startCell) {
	GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
	ASSERT(gObj != nullptr, "Object with id %d exists", id);

	GridNavigator* gNav = gObj->GetComponent<GridNavigator>();
	ASSERT(gNav != nullptr, "Object with id %d has GridNavigator component", id);

	GridCell* unitCell = startCell;
	if (unitCell == nullptr) {
		unitCell = gNav->GetCurrentCell();
	}

	int destCellX1, destCellZ1, destCellX2, destCellZ2;
	this->GetBothDestinationCoordinates(destCellX1, destCellZ1, destCellX2, destCellZ2);
	GridCell* destCell1 = SINGLETONS->GetGridManager()->GetGrid()->GetCell(destCellX1, destCellZ1);
	GridCell* destCell2 = SINGLETONS->GetGridManager()->GetGrid()->GetCell(destCellX2, destCellZ2);
	GridCell* target;
	// Send the unit to the destination cell further from its starting point.
	if(glm::distance(unitCell->center, destCell1->center) < glm::distance(unitCell->center, destCell2->center)) {
		target = destCell2;
	} else {
		target = destCell1;
	}

	BaseUnit* pU = gObj->GetComponent<BaseUnit>();
	ASSERT(pU != nullptr, "gameObject has a BaseUnit component");
	if(pU != nullptr) {
		pU->OnTransitionZoneEntered(target);
	}

	if(id == SINGLETONS->GetGridManager()->GetSelectedUnitId()) {
		GridNavigator* gNav = gObj->GetComponent<GridNavigator>();
		ASSERT(gNav != nullptr, "gameObject has a GridNavigator component");
		if (gNav != nullptr) {
			if (gNav->CanReachDestination(target)) {
				ShadyCamera* shadyCam = ENGINE->GetSceneGraph3D()->GetMainCamera()->GetObject()->GetComponent<ShadyCamera>();
				ASSERT(shadyCam != nullptr, "What happened to the shady cam?");
				if (shadyCam != nullptr) {
					shadyCam->MoveGameCamToRoom(target->x, target->z);
				}
			}
		}
	}
}
