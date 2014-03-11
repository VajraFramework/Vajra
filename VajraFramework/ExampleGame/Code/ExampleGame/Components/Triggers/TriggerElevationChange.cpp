//
//  TriggerElevationChange.cpp
//  Created by Matt Kaufmann on 02/15/14.
//
#include "ExampleGame/Components/GameScripts/Units/Assassin.h"
#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/Grid/GridZone.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/Components/Triggers/TriggerElevationChange.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"

#include "Libraries/glm/gtx/vector_angle.hpp"

void elevationChangeTweenCallback(ObjectIdType gameObjectId, std::string /*tweenClipName*/) {
	GameObject* caller = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);

	// Make sure the switch is still around
	if (caller != nullptr) {
		TriggerElevationChange* triggerComp = caller->GetComponent<TriggerElevationChange>();
		ASSERT(triggerComp != nullptr, "elevationChangeNumberTweenCallback: Object %d has TriggerElevationChange component", gameObjectId);
		if (triggerComp != nullptr) {
			// Update the grid cells
			if (!triggerComp->reversed) {
				triggerComp->changeCellElevations(triggerComp->isToggled);
			}
			triggerComp->setCellsInGridZonePassable(true);

			triggerComp->unchildObjectsFromElevator();

			// Navigators need to update their pathfinding
			MessageChunk gridNavMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			gridNavMessage->SetMessageType(MESSAGE_TYPE_GRID_NAVIGATION_REFRESH);
			ENGINE->GetMessageHub()->SendMulticastMessage(gridNavMessage, SINGLETONS->GetGridManagerObject()->GetId());
		}
	}
}

TriggerElevationChange::TriggerElevationChange() : Triggerable() {
	this->init();
}

TriggerElevationChange::TriggerElevationChange(Object* object_) : Triggerable(object_) {
	this->init();
}

TriggerElevationChange::~TriggerElevationChange() {
	this->destroy();
}

void TriggerElevationChange::init() {
	this->gameObjectRef = (GameObject*)this->GetObject();
	ASSERT(this->gameObjectRef->GetClassType() & CLASS_TYPE_GAMEOBJECT, "Object is a game object");

	this->elevationChange = 1;
	this->transitTime     = 1.0f;
	this->reversed        = false;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT, this->GetTypeId(), true);
}

void TriggerElevationChange::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void TriggerElevationChange::SetTriggerType(std::string typeStr) {
	Triggerable::SetTriggerType(typeStr);
}

void TriggerElevationChange::HandleMessage(MessageChunk messageChunk) {
	Triggerable::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT:
			for (auto iter = this->unitsInZone.begin(); iter != this->unitsInZone.end(); ++iter) {
				// If it's the selected unit the camera should follow it up
				if(*iter == SINGLETONS->GetGridManager()->GetSelectedUnitId()) {
					SINGLETONS->GetGridManager()->GetShadyCamera()->FollowGameObjectDirectly(this->GetObject()->GetId());
				}
			}
			break;
	}
}

void TriggerElevationChange::SubscribeToMySwitch() {
	Triggerable::SubscribeToMySwitch();
}

void TriggerElevationChange::SubscribeToParentSwitch() {
	Triggerable::SubscribeToParentSwitch();
}

void TriggerElevationChange::onSwitchToggled(bool switchState) {
	this->startTransition(switchState);
}

void TriggerElevationChange::onUnitEnteredZone(ObjectIdType id) {
	auto it = std::find(this->unitsInZone.begin(), this->unitsInZone.end(), id);
	if (it == this->unitsInZone.end()) {
		GameObject* unitObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
		ASSERT(unitObj != nullptr, "Object exists with id %d", id);
		if (unitObj != nullptr) {
			this->unitsInZone.push_back(id);
		}
	}
	else {
		FRAMEWORK->GetLogger()->dbglog("Unit with id %d is already in zone %d", id, this->GetObject()->GetId());
	}
}

void TriggerElevationChange::onUnitExitedZone(ObjectIdType id) {
	auto it = std::find(this->unitsInZone.begin(), this->unitsInZone.end(), id);
	if (it != this->unitsInZone.end()) {
		GameObject* unitObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
		ASSERT(unitObj != nullptr, "Object exists with id %d", id);
		if (unitObj != nullptr) {
			this->unitsInZone.erase(it);
		}
	}
	else {
		FRAMEWORK->GetLogger()->dbglog("Warning: Unit with id %d left zone %d without being in it", id, this->GetObject()->GetId());
	}
}

void TriggerElevationChange::startTransition(bool raised) {
	if (raised != this->isToggled) {
		this->startPositionTween(raised);
		this->setCellsInGridZonePassable(false);
	}
}

void TriggerElevationChange::startPositionTween(bool raised) {
	ObjectIdType myId = this->GetObject()->GetId();
	Transform* trans = this->GetObject()->GetComponent<Transform>();

	// Check if there's already a tween going on.
	OnGoingTransformTweenDetails* translationDetails = ENGINE->GetTween()->GetOnGoingTransformTweenDetails(myId, TRANSFORM_TWEEN_TARGET_POSITION);
	if (translationDetails == nullptr) {
		this->childObjectsToElevator();

		glm::vec3 finalPosition = this->GetObject()->GetComponent<Transform>()->GetPosition();
		if (raised) {
			finalPosition.y += SINGLETONS->GetGridManager()->GetGrid()->ConvertElevationToWorldY(this->elevationChange);
		}
		else {
			finalPosition.y -= SINGLETONS->GetGridManager()->GetGrid()->ConvertElevationToWorldY(this->elevationChange);
		}

		float tweenTime = this->transitTime;

		ENGINE->GetTween()->TweenPosition(
			myId,
			finalPosition,
			tweenTime,
			true,
			INTERPOLATION_TYPE_LINEAR,
			false,
			elevationChangeTweenCallback
		);

		reversed = false;
	}
	else {
		// Reverse the current tween
		float tweenTime = this->transitTime - translationDetails->totalTime + translationDetails->currentTime;
		glm::vec3 diff = glm::vec3(0.0f, SINGLETONS->GetGridManager()->GetGrid()->ConvertElevationToWorldY(this->elevationChange), 0.0f);
		if (raised) {
			diff = diff * (tweenTime / this->transitTime);
		}
		else {
			diff = diff * (-tweenTime / this->transitTime);
		}

		glm::vec3 finalPosition = trans->GetPosition() + diff;

		if (tweenTime > 0.0f) {
			ENGINE->GetTween()->TweenPosition(
				myId,
				finalPosition,
				tweenTime,
				true,
				INTERPOLATION_TYPE_LINEAR,
				false,
				elevationChangeTweenCallback
			);
		}
		else {
			// Well that's strange. Just cancel the tween.
			ENGINE->GetTween()->CancelPostitionTween(myId);
		}

		reversed = !reversed;
	}
}

void TriggerElevationChange::setCellsInGridZonePassable(bool pass) {
	int west, east, south, north;
	// Get the grid zone component on this object.
	GridZone* zone = this->GetObject()->GetComponent<GridZone>();
	ASSERT(zone != nullptr, "Object %d with TriggerElevationChange component also has GridZone component", this->GetObject()->GetId());

	zone->GetZoneBounds(west, east, south, north);

	GameGrid* grid = SINGLETONS->GetGridManager()->GetGrid();
	for (int x = west; x <= east; ++x) {
		for (int z = south; z <= north; ++z) {
			int elevation = grid->GetCellGroundLevel(x, z);
			grid->SetCellPassableAtElevation(x, z, elevation, pass);
		}
	}

	// Navigators need to update their pathfinding
	MessageChunk gridNavMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	gridNavMessage->SetMessageType(MESSAGE_TYPE_GRID_NAVIGATION_REFRESH);
	ENGINE->GetMessageHub()->SendMulticastMessage(gridNavMessage, SINGLETONS->GetGridManagerObject()->GetId());
}

void TriggerElevationChange::changeCellElevations(bool raised) {
	int west, east, south, north;
	// Get the grid zone component on this object.
	GridZone* zone = this->GetObject()->GetComponent<GridZone>();
	ASSERT(zone != nullptr, "Object %d with TriggerElevationChange component also has GridZone component", this->GetObject()->GetId());

	zone->GetZoneBounds(west, east, south, north);

	GameGrid* grid = SINGLETONS->GetGridManager()->GetGrid();
	int diff;
	if (raised) {
		diff = this->elevationChange;
	}
	else {
		diff = -this->elevationChange;
	}
	for (int x = west; x <= east; ++x) {
		for (int z = south; z <= north; ++z) {
			grid->ChangeCellGroundLevel(x, z, diff);
		}
	}
}

void TriggerElevationChange::childObjectsToElevator() {
	// Find all occupants of the cells covered by the elevator and child them to it.
	GridZone* zone = this->GetObject()->GetComponent<GridZone>();
	if (zone != nullptr) {
		int west, east, south, north;
		zone->GetZoneBounds(west, east, south, north);
		for (int x = west; x <= east; ++x) {
			for (int z = south; z <= north; ++z) {
				GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(x, z);
				if (cell != nullptr) {
					for (int y = 0; y < NUM_ELEVATIONS; ++y) {
						ObjectIdType occId = cell->GetOccupantIdAtElevation(y);
						if (occId != OBJECT_ID_INVALID) {
							// Just to be safe, make sure that the object is actually positioned within the zone bounds.
							GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(occId);
							if (gObj != nullptr) {
								glm::vec3 pos = gObj->GetTransform()->GetPositionWorld();
								GridCell* actualCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(pos);
								if (actualCell != nullptr) {
									if ((actualCell->x >= west) && (actualCell->x <= east) && (actualCell->z >= south) && (actualCell->z <= north)) {
										this->unitsInZone.push_back(occId);
										this->gameObjectRef->AddChild_maintainTransform(occId);

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
					}
				}
			}
		}
	}
}

void TriggerElevationChange::unchildObjectsFromElevator() {
	// Unchild all units in zone
	for (auto iter = this->unitsInZone.begin(); iter != this->unitsInZone.end(); ++iter) {
		GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(*iter);
		ASSERT(gObj != nullptr, "Object exists with id %d", *iter);
		if (gObj != nullptr) {
			gObj->GetParentSceneGraph()->GetRootGameObject()->AddChild_maintainTransform(*iter);

		}

		// Re-enable the navigators.
		GridNavigator* gNav = gObj->GetComponent<GridNavigator>();
		if (gNav != nullptr) {
			gNav->EnableNavigation();
		}
	}
	this->unitsInZone.clear();
}
