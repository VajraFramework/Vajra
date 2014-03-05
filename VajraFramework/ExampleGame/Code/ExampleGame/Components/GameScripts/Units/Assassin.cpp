#include "ExampleGame/Components/GameScripts/Units/Assassin.h"
#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridCell.h"
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
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

// Tween callbacks
void assassinTweenCallback(ObjectIdType gameObjectId , std::string /* tweenClipName */) {
	GameObject* go = ENGINE->GetSceneGraph3D()->GetGameObjectById(gameObjectId);
	ASSERT(go != nullptr, "Game object id passed into playerUnitNuumberTweenCallback is not valid");
	if(go != nullptr) {
		Assassin* pUnit = go->GetComponent<Assassin>();
		ASSERT(pUnit != nullptr, "Game object passed into playerUnitNuumberTweenCallback doesn't have a player unit");
		if(pUnit != nullptr) {
			pUnit->specialUpdate(); // make sure we hit everything we got in the last frame
			pUnit->onSpecialEnd();
		}
	}
	
}

void assassinNumberTweenCallback(float /* fromNumber */, float /* toNumber */, float /*currentNumber*/, std::string /*tweenClipName*/, MessageData1S1I1F* userParams) {
	GameObject* go = ENGINE->GetSceneGraph3D()->GetGameObjectById(userParams->i);
	ASSERT(go != nullptr, "Game object id passed into playerUnitNuumberTweenCallback is not valid");
	if(go != nullptr) {
		Assassin* pUnit = go->GetComponent<Assassin>();
		ASSERT(pUnit != nullptr, "Game object passed into playerUnitNuumberTweenCallback doesn't have a player unit");
		if(pUnit != nullptr) {
			pUnit->specialUpdate();
		}
	}
}

Assassin::Assassin() : PlayerUnit() {
	this->init();
}

Assassin::Assassin(Object* object_) : PlayerUnit(object_) {
	this->init();
}

Assassin::~Assassin() {
	this->destroy();
}

void Assassin::init() {
	this->unitType = UnitType::UNIT_TYPE_ASSASSIN;

	{
		// create the arrow tail
		this->arrowTail = new GameObject(ENGINE->GetSceneGraph3D());
		SpriteRenderer* spriteRenderer = this->arrowTail->AddComponent<SpriteRenderer>();
		std::vector<std::string> pathsToTextures;
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Assassin_ArrowStem_magenta_02.png");
		spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);
		this->arrowTail->GetTransform()->SetScale( glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)));
		this->arrowTail->GetTransform()->Rotate(90.0f inRadians, XAXIS);
		this->arrowTail->SetVisible(false);
	}
	{
		// create the arrow head
		this->arrowHead = new GameObject(ENGINE->GetSceneGraph3D());
		SpriteRenderer* spriteRenderer = this->arrowHead->AddComponent<SpriteRenderer>();
		std::vector<std::string> pathsToTextures;
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Assassin_Arrow_magenta_03.png");
		spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);
		this->arrowHead->GetTransform()->Rotate(90.0f inRadians, XAXIS);
		this->arrowHead->SetVisible(false);
	}

	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_CELL_CHANGED, this->GetTypeId(), false);
}

void Assassin::destroy() {
}

void Assassin::HandleMessage(MessageChunk messageChunk) {
	PlayerUnit::HandleMessage(messageChunk);
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_GRID_CELL_CHANGED:
			this->onGridCellChanged(messageChunk->GetSenderId(), messageChunk->messageData.iv1.x, messageChunk->messageData.iv1.z);
			break;

		default:
			break;
	}
}

bool Assassin::isSpecialTouch(int touchId) {
	if(this->getTouchNearUnit()) {
		Touch touch = ENGINE->GetInput()->GetTouch(touchId);
		if(touch.timeDown <= GetFloatGameConstant(GAME_CONSTANT_swipe_duration_in_seconds) && glm::distance(touch.pos, this->touchStartPos) > GetFloatGameConstant(GAME_CONSTANT_assassin_swipe_dist_in_pixels)) {
			this->swipeDirectionScreen = this->touchStartPos - touch.pos;
			this->targetedCell = nullptr;
			this->gridNavRef->HaltMovement();
			this->aimSpecial(touchId);
			return true;
		}
	}
	return false;
}

void Assassin::onSpecialTouch(int touchId) {
	Touch touch = ENGINE->GetInput()->GetTouch(touchId);
	if(touch.phase == TouchPhase::Ended) {
		this->trySpecial(touchId);
	} else if(touch.phase == TouchPhase::Cancelled) {
		this->cancelSpecial();
	} else if(touch.phase == TouchPhase::Moved) {
		this->aimSpecial(touchId);
	}
}

void Assassin::trySpecial(int /*touchId*/) {
	if(this->targetedCell != nullptr && this->targetedCell != this->gridNavRef->GetCurrentCell()) {
		this->startSpecial();
	} else {
		this->cancelSpecial();
	}
}

void Assassin::startSpecial() {
	PlayerUnit::startSpecial();
	
	//this->gridNavRef->SetMovementSpeed(GetFloatGameConstant(GAME_CONSTANT_assassin_attack_speed));
	//this->gridNavRef->SetDestination(this->targetedCell, true);
	this->lastHitCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(this->gameObjectRef->GetTransform()->GetPosition());
	ENGINE->GetTween()->TweenPosition(this->gameObjectRef->GetId(),
									  this->gameObjectRef->GetTransform()->GetPosition(),
									  this->targetLoc,
									  .5f,
									  false,
									  TWEEN_TRANSLATION_CURVE_TYPE_LINEAR,
									  false,
									  assassinTweenCallback);
	MessageData1S1I1F* userParams = new MessageData1S1I1F();
 	userParams->i = this->GetObject()->GetId();
	ENGINE->GetTween()->TweenToNumber(0.0f, 1.0f, .5f, true, false, true, "dash", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D, userParams, assassinNumberTweenCallback);
	
	this->startTouchIndicatorPulse();
	this->arrowHead->SetVisible(false);
	this->arrowTail->SetVisible(false);
}

void Assassin::onSpecialEnd() {
	PlayerUnit::onSpecialEnd();
	this->arrowHead->SetVisible(false);
	this->arrowTail->SetVisible(false);
	//this->gridNavRef->SetMovementSpeed(MOVE_SPEED);
	this->gridNavRef->SetGridPosition(this->targetedCell);
	//(SINGLETONS->GetGridManager()->GetGrid()->GetCell(this->gameObjectRef->GetTransform()->GetPosition()));
	this->gameObjectRef->GetTransform()->SetPosition(this->targetLoc);
}

void Assassin::cancelSpecial() {
	PlayerUnit::cancelSpecial();
	this->arrowHead->SetVisible(false);
	this->arrowTail->SetVisible(false);
	this->gridNavRef->SetMovementSpeed(MOVE_SPEED);
}

void Assassin::onGridCellChanged(ObjectIdType id, int gridX, int gridZ) {
	// If this object is the one that sent the message, broadcast an attack message as well
	if (id == this->GetObject()->GetId()) {
		GridCell* cell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(gridX, gridZ);
		if (cell != nullptr) {
			if (this->GetUnitActionState() == UnitActionState::UNIT_ACTION_STATE_DOING_SPECIAL) {
				// Send an attack message
				MessageChunk attackMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
				attackMessage->SetMessageType(MESSAGE_TYPE_UNIT_SPECIAL_HIT);
				attackMessage->messageData.iv1.x = cell->x;
				attackMessage->messageData.iv1.y = cell->y;
				attackMessage->messageData.iv1.z = cell->z;
				attackMessage->messageData.fv1 = this->specialStartPos;
				ENGINE->GetMessageHub()->SendMulticastMessage(attackMessage, this->GetObject()->GetId());
			}
		}
	}
}

void Assassin::aimSpecial(int touchId){
	this->targetLoc = SINGLETONS->GetGridManager()->TouchPositionToGridPositionAtElevation(ENGINE->GetInput()->GetTouch(touchId).pos, this->gridNavRef->GetCurrentCell()->y);
	glm::vec3 sinPos = this->gameObjectRef->GetTransform()->GetPosition();
	
	std::list<GridCell*> touchedCells;
	GridCell* touchedCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(this->targetLoc);
	ASSERT(touchedCell != nullptr, "the target cell is not null");

	SINGLETONS->GetGridManager()->GetGrid()->TouchedCells(sinPos, this->targetLoc, touchedCells);
	int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(this->gridNavRef->GetCurrentCell()->center.y);
	int cellIndex = 0;
	for( GridCell* c : touchedCells) {
		if(glm::distance(c->center, sinPos) <= GetFloatGameConstant(GAME_CONSTANT_dash_distance_in_units) && SINGLETONS->GetGridManager()->GetGrid()->IsCellPassableAtElevation(c->x, c->z, elevation)) {
			this->targetedCell = c;
		} else {
			break;
		}
		cellIndex++;
	}
	if(this->targetedCell != nullptr && touchedCell != nullptr && this->targetedCell != this->gridNavRef->GetCurrentCell()) {
		glm::vec3 attackDir = glm::normalize(this->targetLoc - sinPos);
		if(touchedCell != this->targetedCell) {
			glm::vec3 temp = this->targetLoc;
			GridCell* tempCell = touchedCell;
			float dis = glm::distance(sinPos, this->targetedCell->center);
			while(tempCell != this->targetedCell && glm::distance(sinPos, temp) > dis) {
				temp -= attackDir * .1f;
				tempCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(temp);
			}
			this->targetLoc = temp;
			touchedCell = tempCell;

		}
		this->SetTouchIndicatorVisible(true);
		this->gridNavRef->SetLookTarget(this->targetLoc);

		// Arrow Tail
		this->arrowTail->SetVisible(true);
		this->SetTouchIndicatorLocation(this->targetLoc);
		this->TouchIndicatorLookAt(targetedCell);

		if(glm::length(attackDir) > GetFloatGameConstant(GAME_CONSTANT_dash_distance_in_units)) {
			this->targetLoc = sinPos + glm::normalize(attackDir) * GetFloatGameConstant(GAME_CONSTANT_dash_distance_in_units);
		}
		float dist = glm::distance(sinPos, this->targetLoc) - 1.5f;
		if(dist < 0.5f) {
			this->arrowTail->SetVisible(false);
		}

		this->arrowTail->GetTransform()->SetScale(1.0f, dist, 1.0f);
	

		Transform* trans = this->arrowTail->GetComponent<Transform>();
		this->GridPlaneLookAt(this->arrowTail, this->targetLoc);

		trans->SetPosition(sinPos + glm::vec3(0.0f, .1f, 0.0f));
		trans->Translate(dist * .5f, attackDir);

		// Arrow Head
		this->arrowHead->SetVisible(true);
		this->GridPlaneLookAt(this->arrowHead, this->targetLoc);
		this->arrowHead->GetTransform()->SetPosition(sinPos + glm::vec3(0.0f, .1f, 0.0f));
		this->arrowHead->GetTransform()->Translate(dist + .5f, attackDir);

	} else {
		this->SetTouchIndicatorVisible(false);
		this->arrowHead->SetVisible(false);
		this->arrowTail->SetVisible(false);
	}
}

void Assassin::specialUpdate() {
	GridCell* currentCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(this->gameObjectRef->GetTransform()->GetPosition());
	if(currentCell != this->lastHitCell) {
		std::list<GridCell*> touchedCells;
		SINGLETONS->GetGridManager()->GetGrid()->TouchedCells(this->lastHitCell, currentCell, touchedCells);
		for( GridCell* c : touchedCells) {
			MessageChunk attackMessage = ENGINE->GetMessageHub()->GetOneFreeMessage();
			attackMessage->SetMessageType(MESSAGE_TYPE_UNIT_SPECIAL_HIT);
			attackMessage->messageData.iv1.x = c->x;
			attackMessage->messageData.iv1.y = c->y;
			attackMessage->messageData.iv1.z = c->z;
			//attackMessage->messageData.fv1 = this->specialStartPos;
			ENGINE->GetMessageHub()->SendMulticastMessage(attackMessage, this->GetObject()->GetId());
		}
		this->lastHitCell = currentCell;
	}
}
