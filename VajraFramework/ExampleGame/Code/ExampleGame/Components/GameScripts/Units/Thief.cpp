#include "ExampleGame/Components/GameScripts/Units/Thief.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"

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
		if(glm::distance(this->targetedCell->center, this->gameObjectRef->GetTransform()->GetPosition()) > 1.0f) {
			this->startSpecial();
		} else {
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
	this->gridNavRef->SetGridPosition(this->targetedCell->center.x, this->targetedCell->center.z);

}