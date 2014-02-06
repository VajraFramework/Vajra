//
//  TransitionZone.cpp
//  Created by Matt Kaufmann on 01/31/14.
//

#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/Grid/GridZones/TransitionZone.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/Messages/Declarations.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
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
			this->onUnitEnteredZone(messageChunk->messageData.i);
			break;
	}
}

void TransitionZone::SetFirstDestination(int cellX, int cellZ) {
	GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(cellX, cellZ);
	this->SetFirstDestination(cell);
}

void TransitionZone::SetFirstDestination(glm::vec3 worldPosition) {
	GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(worldPosition);
	this->SetFirstDestination(cell);
}

void TransitionZone::SetFirstDestination(GridCell* cell) {
	this->destCell1 = cell;
}

void TransitionZone::SetSecondDestination(int cellX, int cellZ) {
	GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(cellX, cellZ);
	this->SetSecondDestination(cell);
}

void TransitionZone::SetSecondDestination(glm::vec3 worldPosition) {
	GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(worldPosition);
	this->SetSecondDestination(cell);
}

void TransitionZone::SetSecondDestination(GridCell* cell) {
	this->destCell2 = cell;
}

void TransitionZone::SetDestinations(int cellX1, int cellZ1, int cellX2, int cellZ2) {
	this->SetFirstDestination(cellX1, cellZ1);
	this->SetSecondDestination(cellX2, cellZ2);
}

void TransitionZone::SetDestinations(glm::vec3 worldPosition1, glm::vec3 worldPosition2) {
	this->SetFirstDestination(worldPosition1);
	this->SetSecondDestination(worldPosition2);
}

void TransitionZone::SetDestinations(GridCell* cell1, GridCell* cell2) {
	this->SetFirstDestination(cell1);
	this->SetSecondDestination(cell2);
}

void TransitionZone::init() {
	this->destCell1 = nullptr;
	this->destCell2 = nullptr;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_ENTERED, this->GetTypeId(), false);
}

void TransitionZone::destroy() {

}

void TransitionZone::onUnitEnteredZone(ObjectIdType id) {
	GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
	ASSERT(gObj != nullptr, "Object with id %d exists", id);

	GridNavigator* gNav = gObj->GetComponent<GridNavigator>();
	ASSERT(gNav != nullptr, "Object with id %d has GridNavigator component", id);

	GridCell* unitCell = gNav->GetCurrentCell();
	GridRoom* unitRoom = SINGLETONS->GetGridManager()->GetGrid()->GetRoom(unitCell);
	GridRoom* destRoom1 = SINGLETONS->GetGridManager()->GetGrid()->GetRoom(destCell1);

	GridCell* target;
	if (unitRoom != destRoom1) {
		// If the first destination and the unit are not in the same room, send the unit to that destination
		target = this->destCell1;
	}
	else {
		// If the first destination is in the same room, send the unit to the other one instead
		// (Hopefully they won't be the same room)
		target = this->destCell2;
	}
	gNav->SetDestination(target);

	ShadyCamera* shadyCam = ENGINE->GetSceneGraph3D()->GetMainCamera()->GetObject()->GetComponent<ShadyCamera>();
	ASSERT(shadyCam != nullptr, "What happened to the shady cam?");
	if (shadyCam != nullptr) {
		shadyCam->MoveToRoom(target);
	}
}
