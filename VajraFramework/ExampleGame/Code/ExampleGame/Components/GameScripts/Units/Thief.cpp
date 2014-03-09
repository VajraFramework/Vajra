#include "ExampleGame/Components/GameScripts/Units/Thief.h"
#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridConstants.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameConstants/GameConstants.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"

#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1I1F.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/ParticleSystems/ParticleSystem.h"
#include "Vajra/Engine/Prefabs/PrefabLoader.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

#define THIEF_SPECIAL_WAY_LOW 0
#define THIEF_SPECIAL_LOW 1
#define THIEF_SPECIAL_MID 2
#define THIEF_SPECIAL_HIGH 3

// Tween callbacks
void thiefNumberTweenCallback(float fromNumber, float toNumber, float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams) {
	GameObject* go = ENGINE->GetSceneGraph3D()->GetGameObjectById(userParams->i);
	ASSERT(go != nullptr, "Game object id passed into playerUnitNuumberTweenCallback is not valid");
	if(go != nullptr) {
		Thief* thief = go->GetComponent<Thief>();
		ASSERT(thief != nullptr, "Game object passed into playerUnitNuumberTweenCallback doesn't have a player unit");
		if(thief != nullptr) {
			if(tweenClipName == "vaultWait") {
				thief->beginPoof(thief->endPoofId);
				go->GetTransform()->SetPosition(thief->targetedCell->center + glm::vec3(0.0f, 1.0f, 0.0f));
				MessageData1S1I1F* params = new MessageData1S1I1F();
 				params->i = userParams->i;
				// this tween should go from 1 - 0 over .5 seconds
				ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, 1.5f, INTERPOLATION_TYPE_LINEAR, true, false, true, "vault", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D, params, thiefNumberTweenCallback);
			} else if(tweenClipName == "vault") {
				if (currentNumber == toNumber) {
					thief->onSpecialEnd();
				} else {
					go->GetTransform()->SetPosition(thief->targetedCell->center + glm::vec3(0.0f, currentNumber, 0.0f));
				}
			}
		}
	}
}

void thiefTweenCallback(ObjectIdType gameObjectId , std::string /* tweenClipName */) {
	GameObject* go = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	ASSERT(go != nullptr, "Game object id passed into thiefTweenCallback is not valid");
	if(go != nullptr) {
		Thief* thief = go->GetComponent<Thief>();
		ASSERT(thief != nullptr, "Game object passed into thiefTweenCallback doesn't have a player unit");
		if(thief != nullptr) {
			thief->beginPoof(thief->startPoofId);
			MessageData1S1I1F* userParams = new MessageData1S1I1F();
 			userParams->i = gameObjectId;
			ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, 0.2f, INTERPOLATION_TYPE_LINEAR, true, false, false, "vaultWait", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D, userParams, thiefNumberTweenCallback);
		}
	}
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
	this->createTouchIndicator();
	this->createPoofEffects();
}

void Thief::destroy() {
	this->deleteTargets();
}

void Thief::amendTouchIndicatorPaths(std::vector<std::string>& pathsToTextures) {
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_TouchIndicator_Success_01.png");
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_TouchIndicator_Fail_01.png");
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_TouchIndicator_Success_01.png");
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_Jump_Full_05.png");
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_Jump_Full_01.png");
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_Jump_Full_03.png");
	
}

bool Thief::isSpecialTouch(int touchId) {
	if(this->getTouchNearUnit()) {
		Touch touch = ENGINE->GetInput()->GetTouch(touchId);
		if(glm::distance(touch.pos, this->touchStartPos) > GetFloatGameConstant(GAME_CONSTANT_thief_swipe_dist_in_pixels) || (touch.timeDown >= GetFloatGameConstant(GAME_CONSTANT_long_press_length_in_seconds) && glm::distance(touch.pos, this->touchStartPos) <= GetFloatGameConstant(GAME_CONSTANT_allowed_finger_movement_in_press))) {
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
		if(std::find(this->legalTargets.begin(), this->legalTargets.end(), this->targetedCell) != this->legalTargets.end()) {
			this->startSpecial();
		} else {
			this->targetedCell = this->gridNavRef->GetCurrentCell();
			this->cancelSpecial();
		}
		this->tweenOutTargets();
	} else if(touch.phase == TouchPhase::Moved) {
		this->aimSpecial(touchId);
	}
}

void Thief::startSpecial() {
	PlayerUnit::startSpecial();
	this->gridNavRef->SetGridPosition(nullptr);

	// Remove the indicator at the selected position
	GameObject* selectedTargetIndicator = this->targetIndicatorsRef[this->targetedCell];
	this->targetIndicatorsRef.erase(this->targetedCell);
	delete selectedTargetIndicator;

	this->GridPlaneSetPos(this->touchIndicatorRef, this->targetedCell);
	this->startTouchIndicatorPulse();
	this->SetTouchIndicatorSprite(PLAYER_NUM_TOUCH_IMAGES + this->textureIndexForElevation(this->targetedCell->y));
	this->SetTouchIndicatorVisible(true);
	
	float jumpDist = glm::distance(this->targetedCell->center, this->gameObjectRef->GetTransform()->GetPosition());
	float jumpTweenTime = jumpDist / GetFloatGameConstant(GAME_CONSTANT_jump_speed_in_units_per_second);
	ENGINE->GetTween()->TweenPosition(this->gameObjectRef->GetId(),
									  this->gameObjectRef->GetTransform()->GetPosition(),
									  this->gameObjectRef->GetTransform()->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f),
									  .5f,
									  false,
									  INTERPOLATION_TYPE_PARABOLA,
									  false,
									  thiefTweenCallback);
}

void Thief::onSpecialEnd() {
	if(this->GetUnitActionState() == UnitActionState::UNIT_ACTION_STATE_DOING_SPECIAL) {
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
}

void Thief::cancelSpecial() {
	PlayerUnit::cancelSpecial();
	this->tweenOutTargets();
	
}
void Thief::aimSpecial(int touchId) {
	GridCell* prevTargetCell = this->targetedCell;
	this->targetedCell = this->GetCurrentTouchedCell();

	if(this->targetIndicatorsRef[prevTargetCell] && this->targetIndicatorsRef[this->targetedCell]) {
		if(this->targetedCell != prevTargetCell) {
			this->scaleDownIndicator(prevTargetCell);
			this->scaleUpIndicator(this->targetedCell);
		}
	} else if(this->targetIndicatorsRef[prevTargetCell]) {
		GridCell* nearCell = this->getNearCellTowardsUnit(touchId);
		if(nearCell != nullptr) {
			if(this->targetIndicatorsRef[nearCell]) {
				if(nearCell != prevTargetCell) {
					this->scaleDownIndicator(prevTargetCell);
					this->scaleUpIndicator(nearCell);
				}
			} else {
				this->scaleDownIndicator(prevTargetCell);
			}
			this->targetedCell = nearCell;
		} else {
			this->scaleDownIndicator(prevTargetCell);
		}
	} else if(this->targetIndicatorsRef[this->targetedCell]) {
		this->scaleUpIndicator(this->targetedCell);
	} else {
		GridCell* nearCell = this->getNearCellTowardsUnit(touchId);
		if(nearCell != nullptr) {
			if(this->targetIndicatorsRef[nearCell]) {
				this->scaleUpIndicator(nearCell);
			}
			this->targetedCell = nearCell;
		} 
	} 

	// Looka the target
	this->gridNavRef->SetLookTarget(this->targetedCell->center);
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
	}
	this->createTargets();
	this->updateTargets();
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
		spriteRenderer->SetHasTransperancy(true);
		std::vector<std::string> pathsToTextures;
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_TouchIndicator_Success_01.png");
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_Jump_Full_05.png");
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_Jump_Full_01.png");
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_Jump_Full_03.png");
		spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);
		indicator->GetTransform()->SetScale( glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)));
		indicator->GetTransform()->Rotate(90.0f inRadians, XAXIS);
		this->targetIndicatorsRef[c] = indicator;
	}
}

void Thief::updateTargets() {
	// TODO [Implement] update legal target cells

	for( GridCell* c : this->legalTargets ) {
		ASSERT(targetIndicatorsRef[c] != nullptr, "target indicator for cell is not null");
		if(c->center != targetIndicatorsRef[c]->GetTransform()->GetPosition()) {
			this->GridPlaneSetPos(targetIndicatorsRef[c], c);
			
			targetIndicatorsRef[c]->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(textureIndexForElevation(c->y));

		}
	}
}

void Thief::createPoofEffects() {
	GameObject* startPoof = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + "thiefPoof.prefab", ENGINE->GetSceneGraph3D());
	this->startPoofId = startPoof->GetId();


	GameObject* endPoof = PrefabLoader::InstantiateGameObjectFromPrefab(FRAMEWORK->GetFileSystemUtils()->GetDevicePrefabsResourcesPath() + "thiefPoof.prefab", ENGINE->GetSceneGraph3D());
	this->endPoofId = endPoof->GetId();
}

void Thief::beginPoof(ObjectIdType poofId) {
	GameObject* poofObj = ENGINE->GetSceneGraph3D()->GetGameObjectById(poofId);
	if (poofObj != nullptr) {
		ParticleSystem* poofParticleSystem = poofObj->GetComponent<ParticleSystem>();
		VERIFY(poofParticleSystem != nullptr, "poof effect prefab has a particle system on it");
		if(!poofParticleSystem->GetIsPlaying()) {
			poofParticleSystem->Play();
			
			// Move the effect to this object's position
			Transform* myTrans = this->gameObjectRef->GetTransform();
			Transform* effectTrans = poofObj->GetTransform();
			effectTrans->SetPosition(myTrans->GetPositionWorld());
		}
	}
}

int Thief::textureIndexForElevation(int cellElevation) {
	int currentElevation = this->gridNavRef->GetCurrentCell()->y;
	int spriteToUse = THIEF_SPECIAL_MID; // default it to mid
	if(currentElevation > cellElevation) {
		if(currentElevation == cellElevation + 1) {
			spriteToUse = THIEF_SPECIAL_LOW;
		} else {
			spriteToUse = THIEF_SPECIAL_WAY_LOW;
		}
	} else if(currentElevation < cellElevation) {
		spriteToUse = THIEF_SPECIAL_HIGH;
	} 
	return spriteToUse;
}
void Thief::deleteTargets() {
	this->targetIndicatorsRef.clear();
}

void Thief::scaleUpIndicator(GridCell* cell) {
	if(cell != nullptr && this->targetIndicatorsRef[cell] != nullptr) {
		ENGINE->GetTween()->TweenScale(this->targetIndicatorsRef[cell]->GetId(), 
									   glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)),
									   glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale_hover)), 
									   GetFloatGameConstant(GAME_CONSTANT_target_tween_time));		
	}
}

void Thief::scaleDownIndicator(GridCell* cell) {
	if(cell != nullptr && this->targetIndicatorsRef[cell] != nullptr) {
		ENGINE->GetTween()->TweenScale(this->targetIndicatorsRef[cell]->GetId(), 
									   glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale_hover)),
									   glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)), 
									   GetFloatGameConstant(GAME_CONSTANT_target_tween_time));		
	}
}

GridCell* Thief::getNearCellTowardsUnit(int touchId) {
	glm::vec3 unitPos = this->gridNavRef->GetCurrentCell()->center;
	glm::vec3 touchPos;
	if(this->GetCurrentTouchedCell()) {
		touchPos = SINGLETONS->GetGridManager()->TouchPositionToGridPositionAtElevation(ENGINE->GetInput()->GetTouch(touchId).pos, this->GetCurrentTouchedCell()->y);
	}  else {
		touchPos = SINGLETONS->GetGridManager()->TouchPositionToGridPositionAtElevation(ENGINE->GetInput()->GetTouch(touchId).pos, this->gridNavRef->GetCurrentCell()->y);
	}
	glm::vec3 dirTowardsThief = glm::normalize(unitPos - touchPos);
	dirTowardsThief *= .55f;
	glm::vec3 cellLocation = touchPos + dirTowardsThief;
	return SINGLETONS->GetGridManager()->GetGrid()->GetCell(cellLocation);
}
