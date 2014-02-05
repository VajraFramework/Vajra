//
//  TransitionZone.cpp
//  Created by Matt Kaufmann on 01/31/14.
//

#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/Grid/GridZones/TransitionZone.h"
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

void TransitionZone::SetDestination(int cellX, int cellZ) {
	GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(cellX, cellZ);
	if (cell != nullptr) {
		this->destX = cell->x;
		this->destZ = cell->z;
	}
}

void TransitionZone::SetDestination(glm::vec3 worldPos) {
	GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(worldPos);
	if (cell != nullptr) {
		this->destX = cell->x;
		this->destZ = cell->z;
	}
}

void TransitionZone::init() {
	this->zoneType = GRID_ZONE_TYPE_TRANSITION;
	this->destX = -1;
	this->destZ = -1;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_ENTERED, this->GetTypeId(), false);
}

void TransitionZone::destroy() {

}

void TransitionZone::onUnitEnteredZone(ObjectIdType id) {
	GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
	ASSERT(gObj != nullptr, "Object with id %d exists", id);

	GridNavigator* gNav = gObj->GetComponent<GridNavigator>();
	ASSERT(gNav != nullptr, "Object with id %d has GridNavigator component", id);

	if (IsCellWithinZone(gNav->GetDestination())) {
		gNav->SetDestination(this->destX, this->destZ);
	}
}
