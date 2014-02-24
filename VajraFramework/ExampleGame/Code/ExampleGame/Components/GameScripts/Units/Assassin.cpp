#include "ExampleGame/Components/GameScripts/Units/Assassin.h"
#include "ExampleGame/Components/Grid/GameGrid.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameConstants/GameConstants.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"

#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
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
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Assassin_ArrowStem_02.png");
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
		pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Assassin_Arrow_05.png");
		spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);
		this->arrowHead->GetTransform()->Rotate(90.0f inRadians, XAXIS);
		this->arrowHead->SetVisible(false);
	}
}

void Assassin::destroy() {
}

bool Assassin::isSpecialTouch(int touchId) {
	if(this->getTouchNearUnit()) {
		Touch touch = ENGINE->GetInput()->GetTouch(touchId);
		if(touch.timeDown <= GetFloatGameConstant(GAME_CONSTANT_swipe_duration_in_seconds) && glm::distance(touch.pos, this->touchStartPos) > GetFloatGameConstant(GAME_CONSTANT_swipe_dist_in_pixels)) {
			this->swipeDirectionScreen = this->touchStartPos - touch.pos;
			this->targetedCell = nullptr;
			this->aimSpecial();
			return true;
		}
	}
	return false;
}

void Assassin::onSpecialTouch(int touchId) {
	Touch touch = ENGINE->GetInput()->GetTouch(touchId);
	if(touch.phase == TouchPhase::Ended) {
		this->trySpecial(touchId);
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
	this->gridNavRef->SetDestination(this->targetedCell->x, this->targetedCell->z);
	
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

void Assassin::touchedCellChanged(GridCell* prevTouchedCell) {
	if(this->inputState != InputState::INPUT_STATE_SPECIAL) {
		PlayerUnit::touchedCellChanged(prevTouchedCell);
	} else {
		this->aimSpecial();
	}
}

void Assassin::aimSpecial(){
	std::list<GridCell*> touchedCells;
	SINGLETONS->GetGridManager()->GetGrid()->TouchedCells(this->gridNavRef->GetCurrentCell(), this->GetCurrentTouchedCell(), touchedCells);
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
	if(this->targetedCell != this->gridNavRef->GetCurrentCell()) {
		this->SetTouchIndicatorVisible(true);

		// Arrow Tail
		this->arrowTail->SetVisible(true);
		this->gridNavRef->SetLookTarget(this->targetedCell->center);
		this->SetTouchIndicatorCell(this->targetedCell);
		this->TouchIndicatorLookAt(this->targetedCell);
		float dist = glm::distance(this->gridNavRef->GetCurrentCell()->center, this->targetedCell->center) - 1.5f;
		if(dist <= .5f) {
			this->arrowHead->SetVisible(false);
		}
		this->arrowTail->GetTransform()->SetScale(1.0f, dist, 1.0f);

		Transform* trans = this->arrowTail->GetComponent<Transform>();
		this->GridPlaneLookAt(this->arrowTail, this->targetedCell);

		trans->SetPosition(this->gridNavRef->GetCurrentCell()->center + glm::vec3(0.0f, .1f, 0.0f));
		trans->Translate(dist * .5f , trans->GetUp());

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
