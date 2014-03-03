#include "ExampleGame/Components/GameScripts/Units/Assassin.h"
#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridCell.h"
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
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

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
	
	this->gridNavRef->SetMovementSpeed(GetFloatGameConstant(GAME_CONSTANT_assassin_attack_speed));
	this->gridNavRef->SetDestination(this->targetedCell, true);
	
	this->startTouchIndicatorPulse();
	this->arrowHead->SetVisible(false);
	this->arrowTail->SetVisible(false);
}

void Assassin::onSpecialEnd() {
	PlayerUnit::onSpecialEnd();
	this->arrowHead->SetVisible(false);
	this->arrowTail->SetVisible(false);
	this->gridNavRef->SetMovementSpeed(MOVE_SPEED);

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
	glm::vec3 targetLoc = SINGLETONS->GetGridManager()->TouchPositionToGridPositionAtElevation(ENGINE->GetInput()->GetTouch(touchId).pos, this->gridNavRef->GetCurrentCell()->y);

	std::list<GridCell*> touchedCells;
	
	GridCell* targetCell = SINGLETONS->GetGridManager()->GetGrid()->GetCell(targetLoc);
	ASSERT(targetCell != nullptr, "the target cell is not null");

	SINGLETONS->GetGridManager()->GetGrid()->TouchedCells(this->gridNavRef->GetCurrentCell(), targetCell, touchedCells);
	int elevation = SINGLETONS->GetGridManager()->GetGrid()->GetElevationFromWorldY(this->gridNavRef->GetCurrentCell()->center.y);
	int cellIndex = 0;
	for( GridCell* c : touchedCells) {
		if(cellIndex <= GetFloatGameConstant(GAME_CONSTANT_dash_distance_in_units) && SINGLETONS->GetGridManager()->GetGrid()->IsCellPassableAtElevation(c->x, c->z, elevation)) {
			this->targetedCell = c;
		} else {
			break;
		}
		cellIndex++;
	}
	if(this->targetedCell != nullptr && this->targetedCell != this->gridNavRef->GetCurrentCell()) {
		this->SetTouchIndicatorVisible(true);

		// Arrow Tail
		this->arrowTail->SetVisible(true);
		this->gridNavRef->SetLookTarget(this->targetedCell->center);
		this->SetTouchIndicatorCell(this->targetedCell);
		this->TouchIndicatorLookAt(this->targetedCell);
		float dist = glm::distance(this->gridNavRef->GetCurrentCell()->center, this->targetedCell->center) - 1.5f;
		if(dist < 0.5f) {
			this->arrowTail->SetVisible(false);
		}
		this->arrowTail->GetTransform()->SetScale(1.0f, dist, 1.0f);

		Transform* trans = this->arrowTail->GetComponent<Transform>();
		this->GridPlaneLookAt(this->arrowTail, this->targetedCell);

		trans->SetPosition(this->gameObjectRef->GetTransform()->GetPosition() + glm::vec3(0.0f, .1f, 0.0f));
		trans->Translate(dist * .5f, trans->GetUp());

		// Arrow Head
		glm::vec3 attackDir = this->targetedCell->center - this->gridNavRef->GetCurrentCell()->center;
		this->arrowHead->SetVisible(true);
		this->GridPlaneLookAt(this->arrowHead, this->targetedCell);
		this->arrowHead->GetTransform()->SetPosition(this->targetedCell->center + glm::vec3(0.0f, .1f, 0.0f));
		this->arrowHead->GetTransform()->Translate(-1.0f, attackDir);

	} else {
		this->SetTouchIndicatorVisible(false);
		this->arrowHead->SetVisible(false);
		this->arrowTail->SetVisible(false);
	}
}
