#include "ExampleGame/Components/GameScripts/Units/Thief.h"
#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridConstants.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameConstants/GameConstants.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"

#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
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
				for(GameObject* go : thief->targetIndicatorsRef ) {

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
			this->gridNavRef->HaltMovement();
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
			this->cancelSpecial();
		}
		this->tweenOutTargets();
	}
}

void Thief::startSpecial() {
	PlayerUnit::startSpecial();
	this->gridNavRef->SetGridPosition(nullptr);

	// Remove the indicator at the selected position
	GameObject* selectedTargetIndicator = this->targetIndicatorsRef[this->targetedCell];
	this->targetIndicatorsRef.erase(this->targetedCell);
	delete selectedTargetIndicator;

	this->SetTouchIndicatorCell(this->targetedCell);
	this->startTouchIndicatorPulse();
	this->SetTouchIndicatorSprite(THIEF_SPECIAL);
	this->SetTouchIndicatorVisible(true);
	
	float jumpDist = glm::distance(this->targetedCell->center, this->gameObjectRef->GetTransform()->GetPosition());
	float jumpTweenTime = jumpDist / GetFloatGameConstant(GAME_CONSTANT_jump_speed_in_units_per_second);
	ENGINE->GetTween()->TweenPosition(this->gameObjectRef->GetId(),
									  this->gameObjectRef->GetTransform()->GetPosition(),
									  this->targetedCell->center,
									  jumpTweenTime,
									  false,
									  TWEEN_TRANSLATION_CURVE_TYPE_PARABOLA, 
									  false,
									  thiefTweenCallback);
 
}

void Thief::onSpecialEnd() {
	PlayerUnit::onSpecialEnd();
	this->gridNavRef->SetGridPosition(this->targetedCell);

	// Broadcast an attack message
	MessageChunk attackMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	attackMessage->SetMessageType(MESSAGE_TYPE_UNIT_SPECIAL_HIT);
	attackMessage->messageData.iv1.x = this->targetedCell->x;
	attackMessage->messageData.iv1.y = this->targetedCell->y;
	attackMessage->messageData.iv1.z = this->targetedCell->z;
	attackMessage->messageData.fv1 = this->specialStartPos;
	ENGINE->GetMessageHub()->SendMulticastMessage(attackMessage, this->GetObject()->GetId());
}

void Thief::cancelSpecial() {
	PlayerUnit::cancelSpecial();
	this->tweenOutTargets();
	
}
void Thief::touchedCellChanged(GridCell* prevTouchedCell) {
	if(this->inputState != InputState::INPUT_STATE_SPECIAL) {
		PlayerUnit::touchedCellChanged(prevTouchedCell);
	} else {
		if(this->targetIndicatorsRef[prevTouchedCell]) {
			ENGINE->GetTween()->TweenScale(this->targetIndicatorsRef[prevTouchedCell]->GetId(), glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale_hover)),
																							 glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)), 
																							 GetFloatGameConstant(GAME_CONSTANT_target_tween_time));
		}
		if(this->targetIndicatorsRef[this->GetCurrentTouchedCell()]) {
			ENGINE->GetTween()->TweenScale(this->targetIndicatorsRef[this->GetCurrentTouchedCell()]->GetId(),  glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)),
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
		int cellElevation = 0;
		for(int i = 0; i < NUM_ELEVATIONS; ++i) {
			if(SINGLETONS->GetGridManager()->GetGrid()->IsCellPassableAtElevation(c->x, c->z, i)) {
				cellElevation = i;
				break;
			}
			
		}

		// The thief can jump to a maximum of one elevation level above her
		if (cellElevation <= (elevation + 1)) {
			if ((cellElevation > elevation) || (SINGLETONS->GetGridManager()->GetGrid()->HasLineOfSight(currentCell, c, elevation))) {
				if (SINGLETONS->GetGridManager()->GetGrid()->IsCellPassableAtElevation(c->x, c->z, cellElevation)) {
					int elevationDiff = elevation - cellElevation;
					if (elevationDiff < 0) {
						elevationDiff = 0;
					}

					float maxRange = fmax(GetFloatGameConstant(GAME_CONSTANT_jump_distance_in_units) + elevationDiff * GetFloatGameConstant(GAME_CONSTANT_jump_elevation_multiplier), 1.0f);
					float dist = SINGLETONS->GetGridManager()->GetGrid()->GetGroundDistanceBetweenCells(currentCell, c);
					if (dist <= maxRange) {
						this->legalTargets.push_back(c);
					}
				}
			}
		}
		/*
		if(cellElevation == elevation) { // is the cell on the same height
			if(this->gridNavRef->CanReachDestination(c, GetFloatGameConstant(GAME_CONSTANT_jump_distance_in_units)) && SINGLETONS->GetGridManager()->GetGrid()->HasLineOfSight(currentCell, c, elevation) ) {
				this->legalTargets.push_back(c);
			}
		}
		else if(cellElevation <= elevation + 1) { // is the cell below it
			if(SINGLETONS->GetGridManager()->GetGrid()->IsCellPassableAtElevation(c->x, c->z, cellElevation)) {
				float dist = glm::distance(glm::vec2(c->x, c->z), glm::vec2(currentCell->x, currentCell->z));
				if(dist <= GetFloatGameConstant(GAME_CONSTANT_jump_distance_in_units) + cellElevation * GetFloatGameConstant(GAME_CONSTANT_jump_elevation_multiplier)) {
					this->legalTargets.push_back(c);
				}
			}
		}
		*/
	}
	this->createTargets();
	this->tweenInTargets();
}

void Thief::tweenInTargets() {
	glm::vec3 pos;
	for(auto contents : this->targetIndicatorsRef ) {
		if(contents.second != nullptr) {
			pos = contents.second->GetTransform()->GetPosition();
			ENGINE->GetTween()->TweenPosition(contents.second->GetId(), pos, pos + glm::vec3(0.0f, GetFloatGameConstant(GAME_CONSTANT_target_indicator_offset), 0.0f), GetFloatGameConstant(GAME_CONSTANT_target_tween_time));
		}
	}
}

void Thief::tweenOutTargets() {
	glm::vec3 pos;
	for(auto contents : this->targetIndicatorsRef ) {
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
		indicator->GetTransform()->Rotate(90.0f inRadians, XAXIS);
		this->targetIndicatorsRef[c] = indicator;
	}
}

void Thief::deleteTargets() {
	this->targetIndicatorsRef.clear();
}
