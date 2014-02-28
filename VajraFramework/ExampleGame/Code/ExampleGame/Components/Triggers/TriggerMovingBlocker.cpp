//
//  TriggerMovingBlocker.cpp
//  Created by Matt Kaufmann on 02/26/14.
//

#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Triggers/TriggerMovingBlocker.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
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
	this->translation   = ZERO_VEC3;
	this->transitTime   = 1.0f;
	this->isPassable    = false;
	this->isInTransit   = false;
	this->currentCell   = nullptr;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
}

void TriggerMovingBlocker::destroy() {

}

void TriggerMovingBlocker::SetTriggerType(std::string typeStr) {
	Triggerable::SetTriggerType(typeStr);
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
		case MESSAGE_TYPE_FRAME_EVENT:
			this->update();
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

void TriggerMovingBlocker::update() {
	if (this->isInTransit) {
		Transform* trans = this->GetObject()->GetComponent<Transform>();
		GridCell* newCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(trans->GetPositionWorld());

		if (newCell != this->currentCell) {
			if (this->isPassable) {
				if (this->currentCell != nullptr) {
					int gX = this->currentCell->x;
					int gZ = this->currentCell->z;
					int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetCellGroundLevel(gX, gZ);
					SINGLETONS->GetGridManager()->GetGrid()->SetCellPassableAtElevation(gX, gZ, elevation, this->isPassable);
				}
			}
			else {
				if (newCell != nullptr) {
					int gX = newCell->x;
					int gZ = newCell->z;
					int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetCellGroundLevel(gX, gZ);
					SINGLETONS->GetGridManager()->GetGrid()->SetCellPassableAtElevation(gX, gZ, elevation, this->isPassable);
				}
			}
			this->currentCell = newCell;
		}
/*
		if ((newCell != this->currentCell) && (newCell != nullptr)) {
			this->currentCell = newCell;
			this->setCellBlocked(this->isToggled);
		}
*/
	}
}

void TriggerMovingBlocker::onSwitchToggled(bool /*switchState*/) {
	this->startTransformation(!this->isToggled);
}

void TriggerMovingBlocker::startTransformation(bool transformed) {
	if (transformed != this->isToggled) {
		Transform* trans = this->GetObject()->GetComponent<Transform>();
		this->currentCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(trans->GetPositionWorld());
		this->startTranslation(transformed);
		this->isToggled = transformed;
		this->isPassable = !this->isPassable;
		this->isInTransit = true;
		//this->setCellBlocked(this->isToggled);
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

			Transform* trans = this->GetObject()->GetComponent<Transform>();
			this->currentCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(trans->GetPositionWorld());

			ENGINE->GetTween()->TweenPosition(
				myId,
				finalPosition,
				tweenTime,
				true,
				TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
				false,
				nullptr
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

		ENGINE->GetTween()->TweenPosition(
			myId,
			finalPosition,
			tweenTime,
			true,
			TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
			false,
			nullptr
		);
	}
}
/*
void TriggerMovingBlocker::setCellPassable(GridCell* cell, bool passable) {
	if (this->currentCell != nullptr) {
		int x = this->currentCell->x;
		int z = this->currentCell->z;
		int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetCellGroundLevel(x, z);
		SINGLETONS->GetGridManager()->GetGrid()->SetCellPassableAtElevation(x, z, elevation, blocked);
	}
}
*/
