#include "ExampleGame/Components/GameScripts/Units/Thief.h"
#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridConstants.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"

#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"


// Tween callback
void thiefTweenCallback(ObjectIdType gameObjectId , std::string /* tweenClipName */) {
	GameObject* go = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	ASSERT(go != nullptr, "Game object id passed into playerUnitNuumberTweenCallback is not valid");
	if(go != nullptr) {
		Thief* pUnit = go->GetComponent<Thief>();
		ASSERT(pUnit != nullptr, "Game object passed into playerUnitNuumberTweenCallback doesn't have a player unit");
		if(pUnit != nullptr) {
			pUnit->onSpecialEnd();
		}
	}
	
}

// constants
#define ALLOWED_FINGER_MOVEMENT_IN_PRESS 10.0f
#define LONG_PRESS_LENGTH_IN_SECONDS 0.5f
#define JUMP_DISTANCE_IN_UNITS 2
#define JUMP_ELEVATION_MULTIPLIER 2

Thief::Thief() : PlayerUnit() {
	this->init();
}

Thief::Thief(Object* object_) : PlayerUnit(object_) {
	this->init();
}

Thief::~Thief() {
	this->destroy();
}

void Thief::init() {
	this->unitType = UnitType::UNIT_TYPE_THIEF;
}

void Thief::destroy() {

}

bool Thief::isSpecialTouch(int touchId) {
	if(this->getTouchNearUnit()) {
		Touch touch = ENGINE->GetInput()->GetTouch(touchId);
		if(touch.timeDown >= LONG_PRESS_LENGTH_IN_SECONDS && glm::distance(touch.pos, this->touchStartPos) <= ALLOWED_FINGER_MOVEMENT_IN_PRESS) {
			this->targetedCell = nullptr;
			this->updateLegalTagets();
			return true;
		}
	}
	return false;
}

void Thief::onSpecialTouch(int touchId) {
	Touch touch = ENGINE->GetInput()->GetTouch(touchId);
	if(touch.phase == TouchPhase::Ended) {
		this->targetedCell = SINGLETONS->GetGridManager()->TouchPositionToCell(touch.pos);
		// TODO [HACK] Remove when the thief can gather legal targets
		if(std::find(this->legalTargets.begin(), this->legalTargets.end(), this->GetCurrentTouchedCell()) != this->legalTargets.end()) {
			this->startSpecial();
		} else {
			this->targetedCell = this->gridNavRef->GetCurrentCell();
			this->onSpecialEnd();
		}
	}
}

void Thief::startSpecial() {
	PlayerUnit::startSpecial();
	ENGINE->GetTween()->TweenPosition(this->gameObjectRef->GetId(),
									  this->gameObjectRef->GetTransform()->GetPosition(),
									  this->targetedCell->center,
									  1.0f,
									  false,
									  TWEEN_TRANSLATION_CURVE_TYPE_PARABOLA, 
									  false,
									  thiefTweenCallback);
 
}

void Thief::onSpecialEnd() {
	PlayerUnit::onSpecialEnd();
	this->gridNavRef->SetGridPosition(this->targetedCell);
}

void Thief::touchedCellChanged() {
	PlayerUnit::touchedCellChanged();
	if(this->inputState == InputState::INPUT_STATE_SPECIAL) {
		if(std::find(this->legalTargets.begin(), this->legalTargets.end(), this->GetCurrentTouchedCell()) != this->legalTargets.end()) {
			this->GetTouchIndicator()->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(GOOD_TOUCH);
		} else {
			this->GetTouchIndicator()->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(BAD_TOUCH);
		}
	}
}

void Thief::updateLegalTagets() {
	this->legalTargets.clear();
	GridCell* currentCell = this->gridNavRef->GetCurrentCell();
	int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(currentCell->center.y);
	
	std::list<GridCell*> cellsInRange;
	SINGLETONS->GetGridManager()->GetGrid()->GetNeighborCells(cellsInRange, this->gridNavRef->GetCurrentCell(), 
															  JUMP_DISTANCE_IN_UNITS + elevation * JUMP_ELEVATION_MULTIPLIER);

	for ( GridCell* c : cellsInRange) {
		if(c->y < elevation) { // is the cell below it

		} else if(c->y + 1 == elevation) { // Is the cell on the elevation the thief's elevation

		} else if(c->y == elevation) { // is the cell on the same height
			if(this->gridNavRef->CanReachDestination(c, JUMP_DISTANCE_IN_UNITS) && SINGLETONS->GetGridManager()->GetGrid()->HasLineOfSight(currentCell, c, elevation) ) {
				this->legalTargets.push_back(c);
			}
		}
	}
}