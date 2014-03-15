#include "ExampleGame/Components/GameScripts/Units/EnemyUnit.h"
#include "ExampleGame/Components/GameScripts/Units/Thief.h"
#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridConstants.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
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


#define UP_TWEEN_TIME .3f
#define DOWN_TWEEN_TIME .1f
// Tween callbacks
void thiefNumberTweenCallback(float /*fromNumber*/, float toNumber, float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams) {
	FRAMEWORK->GetLogger()->dbglog("\nthiefNumberTweenCallback: %s: %f", tweenClipName.c_str(), currentNumber);
	GameObject* go = ENGINE->GetSceneGraph3D()->GetGameObjectById(userParams->i);
	//ASSERT(go != nullptr, "Game object id passed into playerUnitNuumberTweenCallback is not valid");
	if(go != nullptr) {
		Thief* thief = go->GetComponent<Thief>();
		ASSERT(thief != nullptr, "Game object passed into playerUnitNuumberTweenCallback doesn't have a player unit");
		if(thief != nullptr) {
			if(tweenClipName == "vault") {
				if (currentNumber == toNumber) {
					thief->onSpecialEnd();
				}/* else {
					go->GetTransform()->SetPosition(thief->targetedCell->center + glm::vec3(0.0f, 1.0f - currentNumber, 0.0f));
				}*/
			}else if(tweenClipName == "vaultWait") {
				MessageData1S1I1F* params = new MessageData1S1I1F();
 				params->i = userParams->i;
 				thief->checkLegalAttack();
				ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, DOWN_TWEEN_TIME, INTERPOLATION_TYPE_LINEAR, true, false, true, "vault", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D, params, thiefNumberTweenCallback);
			} else if(tweenClipName == "tweenInFinished") {
				thief->tweenInFinished = true;
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
		if(thief != nullptr) 
			thief->beginPoof(thief->startPoofId);{
			MessageData1S1I1F* userParams = new MessageData1S1I1F();
 			userParams->i = gameObjectId;
			ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, 0.05f, INTERPOLATION_TYPE_LINEAR, true, false, false, "vaultWait", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D, userParams, thiefNumberTweenCallback);
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
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SCENE_START, this->GetTypeId(), false);
	this->unitType = UnitType::UNIT_TYPE_THIEF;
	this->createTouchIndicator();
	this->createPoofEffects();
}

void Thief::destroy() {
	this->deleteTargets();
}

void Thief::start() {
	this->createTargets();
}

void Thief::amendTouchIndicatorPaths(std::vector<std::string>& pathsToTextures) {
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_TouchIndicator_Success_01.png");
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_TouchIndicator_Fail_01.png");
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_Jump_Full_06.png");
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
			this->setIndicators();
			this->updateTargets();
			this->tweenInTargets();
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
	} else {
		this->aimSpecial(touchId);
	}
}

void Thief::startSpecial() {
	PlayerUnit::startSpecial();
	//this->gridNavRef->SetGridPosition(nullptr);
	this->gridNavRef->DisableNavigation();

	// Remove the indicator at the selected position
	this->activeTargetIndicators[this->targetedCell]->SetVisible(false);

	this->GridPlaneSetPos(this->touchIndicatorRef, this->targetedCell);
	this->startTouchIndicatorPulse();
	this->SetTouchIndicatorSprite(PLAYER_NUM_TOUCH_IMAGES + this->textureIndexForElevation(this->targetedCell->y));
	this->SetTouchIndicatorVisible(true);

	//float jumpDist = glm::distance(this->targetedCell->center, this->gameObjectRef->GetTransform()->GetPosition());
	//float jumpTweenTime = jumpDist / GetFloatGameConstant(GAME_CONSTANT_jump_speed_in_units_per_second);
	glm::vec3 jumpOffset = this->gameObjectRef->GetTransform()->GetForward() * .2f + glm::vec3(0.0f, 1.0f, 0.0f);

	ENGINE->GetTween()->TweenPosition(this->gameObjectRef->GetId(),
									  this->gameObjectRef->GetTransform()->GetPosition(),
									  this->gameObjectRef->GetTransform()->GetPosition() + jumpOffset,
									  UP_TWEEN_TIME,
									  false,
									  INTERPOLATION_TYPE_PARABOLA,
									  false,
									  thiefTweenCallback);
}

void Thief::onSpecialEnd() {
	if(this->GetUnitActionState() == UnitActionState::UNIT_ACTION_STATE_DOING_SPECIAL) {
		PlayerUnit::onSpecialEnd();
		this->gridNavRef->SetGridPosition(this->targetedCell);
		this->gridNavRef->EnableNavigation();

		for( GridCell* c : this->legalTargets ) {
			ASSERT(activeTargetIndicators[c] != nullptr, "target indicator for cell is not null");
			activeTargetIndicators[c]->SetVisible(false);
			
		}
		SINGLETONS->GetGridManager()->GetShadyCamera()->MoveGameCamToRoom(this->gridNavRef->GetCurrentCell()->x, this->gridNavRef->GetCurrentCell()->z);
		
	}
}

void Thief::cancelSpecial() {
	PlayerUnit::cancelSpecial();
	this->tweenOutTargets();
	// Place the thief on the ground.
	this->gameObjectRef->GetTransform()->SetPositionWorld(this->gridNavRef->GetCurrentCell()->center);
	this->gridNavRef->EnableNavigation();
	
}
void Thief::aimSpecial(int touchId) {
	if(tweenInFinished) {
		this->updateTargets();
	}

	GridCell* prevTargetCell = this->targetedCell;
	this->targetedCell = this->GetCurrentTouchedCell();

	if(std::find(this->legalTargets.begin(), this->legalTargets.end(), prevTargetCell) == this->legalTargets.end()){
		prevTargetCell = nullptr;
	}
	
	if(std::find(this->legalTargets.begin(), this->legalTargets.end(), this->targetedCell) == this->legalTargets.end()){
		this->targetedCell = nullptr;
	}

	if(this->activeTargetIndicators[prevTargetCell] && this->activeTargetIndicators[this->targetedCell]) {
		if(this->targetedCell != prevTargetCell) {
			this->scaleDownIndicator(prevTargetCell);
			this->scaleUpIndicator(this->targetedCell);
		}
	} else if(this->activeTargetIndicators[prevTargetCell]) {
		GridCell* nearCell = this->getNearCellTowardsUnit(touchId);
		if(nearCell != nullptr) {
			if(this->activeTargetIndicators[nearCell]) {
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
	} else if(this->activeTargetIndicators[this->targetedCell]) {
		this->scaleUpIndicator(this->targetedCell);
	} else {
		GridCell* nearCell = this->getNearCellTowardsUnit(touchId);
		if(nearCell != nullptr) {
			if(this->activeTargetIndicators[nearCell]) {
				this->scaleUpIndicator(nearCell);
			}
			this->targetedCell = nearCell;
		} 
	} 

	// Looka the target
	this->gridNavRef->SetLookTarget(this->targetedCell->center);
}

void Thief::updateLegalTagets() {
	
}

void Thief::checkLegalAttack() {
	//bool attackIsLegal = true;
	if (this->targetedCell != nullptr) {
		GridCell* originCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(this->specialStartPos);
		std::list<GridCell*> touchedCells;
		SINGLETONS->GetGridManager()->GetGrid()->TouchedCells(originCell, this->targetedCell, touchedCells);
		int elevation;
		for (auto iter = touchedCells.rbegin(); iter != touchedCells.rend(); ++iter) {
			GridCell* cell = *iter;
			elevation = SINGLETONS->GetGridManager()->GetGrid()->GetCellGroundLevel(cell->x, cell->z);
			if (SINGLETONS->GetGridManager()->GetGrid()->IsCellPassableAtElevation(cell->x, cell->z, elevation)) {
				ObjectIdType occId = cell->GetOccupantIdAtElevation(elevation);
				if (occId != OBJECT_ID_INVALID) {
					GameObject* occupant = ENGINE->GetSceneGraph3D()->GetGameObjectById(occId);
					if (occupant != nullptr) {
						BaseUnit* unit = occupant->GetComponent<BaseUnit>();
						if (unit != nullptr) {
							if (!unit->CanBeKilledBy(this->GetObject()->GetId(), this->specialStartPos)) {
								continue;
							}
						}
					}
				}
			}
			else {
				continue;
			}
			this->targetedCell = cell;
			break;
		}
		this->gameObjectRef->GetTransform()->SetPositionWorld(this->targetedCell->center + glm::vec3(0.0f, 1.0f, 0.0f));
		this->sendAttackMessage(this->targetedCell->x, this->targetedCell->z, elevation);
		this->beginPoof(this->endPoofId);
	}
}

void Thief::sendAttackMessage(int gridX, int gridZ, int elevation) {
	// Attack the cell
	MessageChunk attackMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
	attackMessage->SetMessageType(MESSAGE_TYPE_GRID_CELL_ENTER_AND_ATTACK);
	attackMessage->messageData.iv1.x = gridX;
	attackMessage->messageData.iv1.y = elevation;
	attackMessage->messageData.iv1.z = gridZ;
	attackMessage->messageData.fv1 = this->specialStartPos;
	ENGINE->GetMessageHub()->SendMulticastMessage(attackMessage, this->GetObject()->GetId());
}

void Thief::tweenInTargets() {
	glm::vec3 pos;
	tweenInFinished = false;
	MessageData1S1I1F* params = new MessageData1S1I1F();
	params->i = this->gameObjectRef->GetId();
	ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, GetFloatGameConstant(GAME_CONSTANT_target_tween_time), INTERPOLATION_TYPE_LINEAR, true, false, true, "tweenInFinished", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D, params, thiefNumberTweenCallback);
	
	for(auto contents : this->activeTargetIndicators ) {
		if(contents.second != nullptr) {
			this->GridPlaneSetPos(activeTargetIndicators[contents.first], contents.first);
			pos = activeTargetIndicators[contents.first]->GetTransform()->GetPosition();
			//pos = contents.first->center;
			ENGINE->GetTween()->TweenScale(contents.second->GetId(), glm::vec3(0), glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)), GetFloatGameConstant(GAME_CONSTANT_target_tween_time));
			//ENGINE->GetTween()->TweenPosition(contents.second->GetId(), pos - glm::vec3(0.0f, 1.5f, 0.0f), pos, GetFloatGameConstant(GAME_CONSTANT_target_tween_time));
		}
	}
}

void Thief::tweenOutTargets() {
	glm::vec3 pos;
	for(auto contents : this->activeTargetIndicators ) {
		if(contents.second != nullptr) {
			pos = contents.second->GetTransform()->GetPosition();
			ENGINE->GetTween()->TweenScale(contents.second->GetId(), glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)), glm::vec3(0), GetFloatGameConstant(GAME_CONSTANT_target_tween_time));
		}
	}
}

void Thief::createTargets() {
	for(int i = 0; i < 60; ++i) {
		GameObject* indicator = new GameObject(ENGINE->GetSceneGraph3D());
		SpriteRenderer* spriteRenderer = indicator->AddComponent<SpriteRenderer>();
		spriteRenderer->SetHasTransperancy(true);
		std::vector<std::string> pathsToTextures;
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_Jump_Full_06.png");
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_Jump_Full_05.png");
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_Jump_Full_01.png");
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_Jump_Full_03.png");
		spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);
		indicator->GetTransform()->SetScale( glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)));
		indicator->GetTransform()->Rotate(90.0f inRadians, XAXIS);
		indicator->SetVisible(false);
		targetIndicators.push_back(indicator);
	}
}

void Thief::setIndicators() {
	for(auto pair : this->activeTargetIndicators) {
		if(pair.second != nullptr) {
			pair.second->SetVisible(false);
		}
	}
	this->activeTargetIndicators.clear();
	this->legalTargets.clear();
	this->cellsInRange.clear();
	SINGLETONS->GetGridManager()->GetGrid()->GetNeighborCells(this->cellsInRange, this->gridNavRef->GetCurrentCell(), 5);
	int i = 0;
	for( GridCell* c : this->cellsInRange ) {
		if(c != nullptr) {
			this->activeTargetIndicators[c] = this->targetIndicators[i];
			i++;
		}
	}
}

void Thief::updateTargets() {
	for( GridCell* c : this->legalTargets ) {
		ASSERT(activeTargetIndicators[c] != nullptr, "target indicator for cell is not null");
		activeTargetIndicators[c]->SetVisible(false);
		
	}
	this->legalTargets.clear();
	GridCell* currentCell = this->gridNavRef->GetCurrentCell();
	int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(currentCell->center.y);
	for ( GridCell* c : cellsInRange) {
		if(c != currentCell) {
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
						float maxRange = fmax((GetFloatGameConstant(GAME_CONSTANT_jump_distance_in_units) + elevationDiff), 1.0f);
						float dist = SINGLETONS->GetGridManager()->GetGrid()->GetGroundDistanceBetweenCells(currentCell, c);
						if (dist <= maxRange) {
							ObjectIdType id = c->GetOccupantIdAtElevation(c->y);
							if(id == OBJECT_ID_INVALID) {
								this->legalTargets.push_back(c);
							} else {
								GameObject* go = ENGINE->GetSceneGraph3D()->GetGameObjectById(id);
								EnemyUnit* eu = go->GetComponent<EnemyUnit>();
								if(eu != nullptr && eu->CanBeKilledBy(this->GetObject()->GetId(), this->gridNavRef->GetCurrentCell()->center)) {
									this->legalTargets.push_back(c);						
								}
							}
						}
					}
				}
			}
		}
	}
	for( GridCell* c : this->legalTargets ) {
		if(activeTargetIndicators.find(c) != activeTargetIndicators.end() && activeTargetIndicators[c] != nullptr) {
			GameObject* obj = activeTargetIndicators[c];
			if(c->center != obj->GetTransform()->GetPosition()) {
				this->GridPlaneSetPos(activeTargetIndicators[c], c);
				activeTargetIndicators[c]->SetVisible(true);
				activeTargetIndicators[c]->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(textureIndexForElevation(c->y));

			}
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
			effectTrans->SetPosition(myTrans->GetPosition());
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
	this->activeTargetIndicators.clear();
}

void Thief::scaleUpIndicator(GridCell* cell) {
	if(cell != nullptr && this->activeTargetIndicators[cell] != nullptr) {
		ENGINE->GetTween()->TweenScale(this->activeTargetIndicators[cell]->GetId(), 
									   glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)),
									   glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale_hover)), 
									   GetFloatGameConstant(GAME_CONSTANT_target_tween_time));		
	}
}

void Thief::scaleDownIndicator(GridCell* cell) {
	if(cell != nullptr && this->activeTargetIndicators[cell] != nullptr) {
		ENGINE->GetTween()->TweenScale(this->activeTargetIndicators[cell]->GetId(), 
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
