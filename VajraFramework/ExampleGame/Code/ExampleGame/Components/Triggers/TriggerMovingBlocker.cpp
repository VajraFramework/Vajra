//
//  TriggerMovingBlocker.cpp
//  Created by Matt Kaufmann on 02/26/14.
//

#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Triggers/TriggerMovingBlocker.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Utilities/MathUtilities.h"

void movingBlockerTweenCallback(ObjectIdType gameObjectId, std::string /*tweenClipName*/) {
	// Make sure the triggerable is still around
	GameObject* caller = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	if (caller != nullptr) {
		TriggerMovingBlocker* blocker = caller->GetComponent<TriggerMovingBlocker>();
		ASSERT(blocker != nullptr, "movingBlockerTweenCallback: caller has TriggerMovingBlocker component");
		if (blocker != nullptr) {
			blocker->isInTransit = false;
		}
	}

	// Navigators need to update their pathfinding
	MessageChunk gridNavMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	gridNavMessage->SetMessageType(MESSAGE_TYPE_GRID_NAVIGATION_REFRESH);
	ENGINE->GetMessageHub()->SendMulticastMessage(gridNavMessage, SINGLETONS->GetGridManagerObject()->GetId());
}

TriggerMovingBlocker::TriggerMovingBlocker() : Triggerable() {
	this->init();
}

TriggerMovingBlocker::TriggerMovingBlocker(Object* object_) : Triggerable(object_) {
	this->init();
}

TriggerMovingBlocker::~TriggerMovingBlocker() {
	this->destroy();
}

void TriggerMovingBlocker::init() {
	this->changeOnEnterCell = true;
	this->changeWalkability = true;
	this->changeVisibility  = true;
	this->translation       = ZERO_VEC3;
	this->transitTime       = 1.0f;
	this->setCellsPassable  = false;
	this->isInTransit       = false;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_ENTERED_CELL, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ZONE_EXITED_CELL, this->GetTypeId(), false);
}

void TriggerMovingBlocker::destroy() {

}

void TriggerMovingBlocker::SetTriggerType(std::string typeStr) {
	Triggerable::SetTriggerType(typeStr);
}

void TriggerMovingBlocker::SetActivationAudio(std::string audioStr) {
	Triggerable::SetActivationAudio(audioStr);
}

void TriggerMovingBlocker::SetDeactivationAudio(std::string audioStr) {
	Triggerable::SetDeactivationAudio(audioStr);
}

void TriggerMovingBlocker::SetToggleState(bool toggle) {
	Triggerable::SetToggleState(toggle);
}

void TriggerMovingBlocker::SetTranslation(float x, float y, float z) {
	glm::vec3 vec(x, y, z);
	this->SetTranslation(vec);
}

void TriggerMovingBlocker::SetTranslation(glm::vec3 vec) {
	this->translation = vec;
}

void TriggerMovingBlocker::HandleMessage(MessageChunk messageChunk) {
	Triggerable::HandleMessage(messageChunk);

	switch (messageChunk->GetMessageType()) {
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

void TriggerMovingBlocker::SubscribeToMySwitch() {
	Triggerable::SubscribeToMySwitch();
}

void TriggerMovingBlocker::SubscribeToParentSwitch() {
	Triggerable::SubscribeToParentSwitch();
}

void TriggerMovingBlocker::SetDecalType(std::string decalType) {
	Triggerable::SetDecalType(decalType);
}

void TriggerMovingBlocker::onSwitchToggled(bool switchState) {
	this->startTransformation(switchState);
}

void TriggerMovingBlocker::startTransformation(bool transformed) {
	if (transformed != this->isToggled) {
		this->startTranslation(transformed);
		this->changeOnEnterCell = !this->changeOnEnterCell;
		this->setCellsPassable = !this->setCellsPassable;
		this->isInTransit = true;
	}
}

void TriggerMovingBlocker::startTranslation(bool transformed) {
	ObjectIdType myId = this->GetObject()->GetId();
	Transform* trans = this->GetObject()->GetComponent<Transform>();

	// Check if there's already a tween going on.
	OnGoingTransformTweenDetails* translationDetails = ENGINE->GetTween()->GetOnGoingTransformTweenDetails(myId, TRANSFORM_TWEEN_TARGET_POSITION);
	if (translationDetails == nullptr) {
		// Don't do a tween if there's no translation
		if (glm::length(this->translation) > ROUNDING_ERROR) {
			glm::vec3 finalPosition = trans->GetPosition();
			if (transformed) {
				finalPosition += this->translation;
			}
			else {
				finalPosition -= this->translation;
			}

			float tweenTime = this->transitTime;

			ENGINE->GetTween()->TweenPosition(
				myId,
				finalPosition,
				tweenTime,
				true,
				INTERPOLATION_TYPE_LINEAR,
				false,
				movingBlockerTweenCallback
			);
		}
	}
	else {
		// Reverse the current tween
		float tweenTime = this->transitTime - translationDetails->totalTime + translationDetails->currentTime;
		glm::vec3 diff;
		if (transformed) {
			diff = this->translation * (tweenTime / this->transitTime);
		}
		else {
			diff = this->translation * (-tweenTime / this->transitTime);
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
				movingBlockerTweenCallback
			);
		}
		else {
			// Well that's strange. Just cancel the tween.
			ENGINE->GetTween()->CancelPostitionTween(myId);
		}
	}
}

void TriggerMovingBlocker::onZoneEnteredCell(int gridX, int gridZ) {
	if (this->changeOnEnterCell) {
		int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetCellGroundLevel(gridX, gridZ);
		if (this->changeWalkability) {
			SINGLETONS->GetGridManager()->GetGrid()->SetCellPassableAtElevation(gridX, gridZ, elevation, this->setCellsPassable);
		}
		if (this->changeVisibility) {
			SINGLETONS->GetGridManager()->GetGrid()->SetCellVisibleAtElevation(gridX, gridZ, elevation, this->setCellsPassable);
		}
	}
}

void TriggerMovingBlocker::onZoneExitedCell(int gridX, int gridZ) {
	if (!this->changeOnEnterCell) {
		int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetCellGroundLevel(gridX, gridZ);
		if (this->changeWalkability) {
			SINGLETONS->GetGridManager()->GetGrid()->SetCellPassableAtElevation(gridX, gridZ, elevation, this->setCellsPassable);
		}
		if (this->changeVisibility) {
			SINGLETONS->GetGridManager()->GetGrid()->SetCellVisibleAtElevation(gridX, gridZ, elevation, this->setCellsPassable);
		}
	}
}
