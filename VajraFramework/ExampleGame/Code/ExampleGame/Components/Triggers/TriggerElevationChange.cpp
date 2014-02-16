//
//  TriggerElevationChange.cpp
//  Created by Matt Kaufmann on 02/15/14.
//

#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/Grid/GridZone.h"
#include "ExampleGame/Components/Triggers/TriggerElevationChange.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"

void elevationChangeTweenCallback(ObjectIdType gameObjectId, std::string /*tweenClipName*/) {
	GameObject* caller = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);

	// Make sure the switch is still around
	if (caller != nullptr) {
		TriggerElevationChange* triggerComp = caller->GetComponent<TriggerElevationChange>();
		ASSERT(triggerComp != nullptr, "elevationChangeNumberTweenCallback: Object %d has TriggerElevationChange component", gameObjectId);
		if (triggerComp != nullptr) {
			int west, east, south, north;
			// Get the grid zone component on this object.
			GridZone* zone = triggerComp->GetObject()->GetComponent<GridZone>();
			ASSERT(zone != nullptr, "Object %d with TriggerElevationChange component also has GridZone component", gameObjectId);

			zone->GetZoneBounds(west, east, south, north);

			GameGrid* grid = SINGLETONS->GetGridManager()->GetGrid();
			int diff;
			triggerComp->isRaised = !triggerComp->isRaised;
			if (triggerComp->isRaised) {
				diff = triggerComp->elevationChange;
			}
			else {
				diff = -triggerComp->elevationChange;
			}
			for (int x = west; x <= east; ++x) {
				for (int z = south; z <= north; ++z) {
					grid->ChangeCellGroundLevel(x, z, diff);
				}
			}

			// Unchild all units in zone
			for (auto iter = triggerComp->unitsInZone.begin(); iter != triggerComp->unitsInZone.end(); ++iter) {
				//glm::vec3 myPos = triggerComp->GetObject()->GetComponent<Transform>()->GetPositionWorld();

				GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(*iter);
				ASSERT(gObj != nullptr, "Object exists with id %d", *iter);
				if (gObj != nullptr) {
					glm::vec3 pos = gObj->GetTransform()->GetPositionWorld();

					gObj->GetParentSceneGraph()->GetRootGameObject()->AddChild(*iter);
					// TODO [Hack] Use SetPositionWorld once implemented
					gObj->GetTransform()->SetPosition(pos);
				}
			}
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
	this->elevationChange = 1;
	this->transitTime     = 1.0f;
	this->isRaised        = false;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_ENTERED, this->GetTypeId(), true);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_EXITED, this->GetTypeId(), true);
}

void TriggerElevationChange::destroy() {

}

void TriggerElevationChange::SetTriggerType(std::string typeStr) {
	Triggerable::SetTriggerType(typeStr);
}

void TriggerElevationChange::SetRaisedState(bool raised) {
	this->isRaised = raised;
}

void TriggerElevationChange::HandleMessage(MessageChunk messageChunk) {
	Triggerable::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_GRID_ZONE_ENTERED:
			this->onUnitEnteredZone(messageChunk->messageData.iv1.x);
			break;

		case MESSAGE_TYPE_GRID_ZONE_EXITED:
			this->onUnitExitedZone(messageChunk->messageData.iv1.x);
			break;
	}
}

void TriggerElevationChange::SubscribeToMySwitch() {
	Triggerable::SubscribeToMySwitch();
}

void TriggerElevationChange::onSwitchToggled(bool /*switchState*/) {
	this->startTransition(!this->isRaised);
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
	if (raised != this->isRaised) {
		// Child all units in zone to this object before tweening.
		for (auto iter = this->unitsInZone.begin(); iter != this->unitsInZone.end(); ++iter) {
			glm::vec3 myPos = this->GetObject()->GetComponent<Transform>()->GetPositionWorld();

			GameObject* gObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(*iter);
			ASSERT(gObj != nullptr, "Object exists with id %d", *iter);
			if (gObj != nullptr) {
				glm::vec3 pos = gObj->GetTransform()->GetPositionWorld();
				this->GetObject()->AddChild(*iter);
				// TODO [Hack] Use SetPositionWorld once implemented
				gObj->GetTransform()->SetPosition(pos - myPos);

				// If it's a unit that's moving, return it to the center of its cell.
				GridNavigator* gNav = gObj->GetComponent<GridNavigator>();
				if (gNav != nullptr) {
					gNav->ReturnToCellCenter();
				}
			}
		}

		std::string tweenName = "ElevationChange";
		tweenName += this->GetObject()->GetId();

		glm::vec3 finalPosition = this->GetObject()->GetComponent<Transform>()->GetPosition();
		if (raised) {
			finalPosition.y += SINGLETONS->GetGridManager()->GetGrid()->ConvertElevationToWorldY(this->elevationChange);
		}
		else {
			finalPosition.y -= SINGLETONS->GetGridManager()->GetGrid()->ConvertElevationToWorldY(this->elevationChange);
		}

		ENGINE->GetTween()->TweenPosition(
			this->GetObject()->GetId(),
			finalPosition,
			this->transitTime,
			true,
			TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
			false,
			elevationChangeTweenCallback
		);
	}
}
