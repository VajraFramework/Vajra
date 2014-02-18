#include "ExampleGame/Components/GameScripts/Units/Thief.h"
#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridConstants.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameConstants/GameConstants.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"

#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"


// Tween callbacks
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

void thiefNumberTweenCallback(float /* fromNumber */, float /* toNumber */, float /*currentNumber*/, std::string /*tweenClipName*/, MessageData1S1I1F* /*userParams*/) {
	/*GameObject* go = ENGINE->GetSceneGraph3D()->GetGameObjectById(userParams->i);
	ASSERT(go != nullptr, "Game object id passed into playerUnitNuumberTweenCallback is not valid");
	if(go != nullptr) {
		Thief* thief = go->GetComponent<Thief>();
		ASSERT(thief != nullptr, "Game object passed into playerUnitNuumberTweenCallback doesn't have a player unit");
		if(thief != nullptr) {
			if(tweenClipName == "targetsIn") {
				for(GameObject* go : thief->targetIndicators ) {

				}
				
			}
		}
	}*/
}

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
	this->deleteTargets();
}

bool Thief::isSpecialTouch(int touchId) {
	if(this->getTouchNearUnit()) {
		Touch touch = ENGINE->GetInput()->GetTouch(touchId);
		if(touch.timeDown >= GetFloatGameConstant(GAME_CONSTANT_long_press_length_in_seconds) && glm::distance(touch.pos, this->touchStartPos) <= GetFloatGameConstant(GAME_CONSTANT_allowed_finger_movement_in_press)) {
			this->targetedCell = nullptr;
			this->SetTouchIndicatorVisible(false);
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
		if(std::find(this->legalTargets.begin(), this->legalTargets.end(), this->GetCurrentTouchedCell()) != this->legalTargets.end()) {
			this->startSpecial();
		} else {
			this->targetedCell = this->gridNavRef->GetCurrentCell();
			this->onSpecialCancelled();
		}
		this->tweenOutTargets();
	}
}

void Thief::startSpecial() {
	PlayerUnit::startSpecial();
	// Remove the indicator at the selected position
	GameObject* selectedTargetIndicator = this->targetIndicators[this->targetedCell];
	this->targetIndicators.erase(this->targetedCell);
	delete selectedTargetIndicator;

	this->SetTouchIndicatorCell(this->targetedCell);
	this->startTouchIndicatorPulse();
	this->SetTouchIndicatorSprite(THIEF_SPECIAL);
	this->SetTouchIndicatorVisible(true);
	
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

void Thief::touchedCellChanged(GridCell* prevTouchedCell) {
	if(this->inputState != InputState::INPUT_STATE_SPECIAL) {
		PlayerUnit::touchedCellChanged(prevTouchedCell);
	} else {
		if(this->targetIndicators[prevTouchedCell]) {
			ENGINE->GetTween()->TweenScale(this->targetIndicators[prevTouchedCell]->GetId(), glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale_hover)), 
																							 glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)), 
																							 GetFloatGameConstant(GAME_CONSTANT_target_tween_time));
		}
		if(this->targetIndicators[this->GetCurrentTouchedCell()]) {
			ENGINE->GetTween()->TweenScale(this->targetIndicators[this->GetCurrentTouchedCell()]->GetId(),  glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)),
																											glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale_hover)), 
																											GetFloatGameConstant(GAME_CONSTANT_target_tween_time));
			this->gridNavRef->SetLookTarget(this->GetCurrentTouchedCell()->center);
		}
		if(std::find(this->legalTargets.begin(), this->legalTargets.end(), this->GetCurrentTouchedCell()) != this->legalTargets.end()) {
			this->SetTouchIndicatorSprite(GOOD_TOUCH);
		} else {
			this->SetTouchIndicatorSprite(BAD_TOUCH);
		}
	}
}

void Thief::updateLegalTagets() {
	this->legalTargets.clear();
	this->deleteTargets();
	GridCell* currentCell = this->gridNavRef->GetCurrentCell();
	int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(currentCell->center.y);
	
	std::list<GridCell*> cellsInRange;
	SINGLETONS->GetGridManager()->GetGrid()->GetNeighborCells(cellsInRange, this->gridNavRef->GetCurrentCell(), 
															  GetFloatGameConstant(GAME_CONSTANT_jump_distance_in_units) 
															  + elevation * GetFloatGameConstant(GAME_CONSTANT_jump_elevation_multiplier));

	for ( GridCell* c : cellsInRange) {
		if(c->y == elevation) { // is the cell on the same height
			if(this->gridNavRef->CanReachDestination(c, GetFloatGameConstant(GAME_CONSTANT_jump_distance_in_units)) && SINGLETONS->GetGridManager()->GetGrid()->HasLineOfSight(currentCell, c, elevation) ) {
				this->legalTargets.push_back(c);
			}
		}
		else if(c->y <= elevation + 1) { // is the cell below it
			if(SINGLETONS->GetGridManager()->GetGrid()->IsCellPassableAtElevation(c->x, c->z, c->y)) {
				float dist = glm::distance(c->origin, currentCell->origin);
				if(dist <= GetFloatGameConstant(GAME_CONSTANT_jump_distance_in_units) + c->y * GetFloatGameConstant(GAME_CONSTANT_jump_elevation_multiplier)) {
					this->legalTargets.push_back(c);
				}
			}
		}
	}
	this->createTargets();
	this->tweenInTargets();
}

void Thief::tweenInTargets() {
	glm::vec3 pos;
	for(auto contents : this->targetIndicators ) {
		if(contents.second != nullptr) {
			pos = contents.second->GetTransform()->GetPosition();
			ENGINE->GetTween()->TweenPosition(contents.second->GetId(), pos, pos + glm::vec3(0.0f, GetFloatGameConstant(GAME_CONSTANT_target_indicator_offset), 0.0f), GetFloatGameConstant(GAME_CONSTANT_target_tween_time));
		}
	}
}

void Thief::tweenOutTargets() {
	glm::vec3 pos;
	for(auto contents : this->targetIndicators ) {
		if(contents.second != nullptr) {
			pos = contents.second->GetTransform()->GetPosition();
			ENGINE->GetTween()->TweenScale(contents.second->GetId(), glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)), glm::vec3(0), GetFloatGameConstant(GAME_CONSTANT_target_tween_time));
		}
	}
}

void Thief::createTargets() {
	for( GridCell* c : this->legalTargets ) {
		GameObject* indicator = new GameObject(ENGINE->GetSceneGraph3D());
		SpriteRenderer* spriteRenderer = indicator->AddComponent<SpriteRenderer>();
		std::vector<std::string> pathsToTextures;
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_Jump_cyan.png");
		spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);
		indicator->GetTransform()->SetPosition(c->center);
		indicator->GetTransform()->SetScale( glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)));
		indicator->GetTransform()->Translate(0.0f, YAXIS);
		indicator->GetTransform()->Rotate(90.0f inRadians, XAXIS);
		this->targetIndicators[c] = indicator;
	}
}

void Thief::deleteTargets() {
	for(auto contents : this->targetIndicators ) {
		if(contents.second != nullptr) {
			delete contents.second;
		}
	}
	this->targetIndicators.clear();
}
