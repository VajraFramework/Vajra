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

	// create the arrow tail
	this->arrowTail = new GameObject(ENGINE->GetSceneGraph3D());
	SpriteRenderer* spriteRenderer = this->arrowTail->AddComponent<SpriteRenderer>();
	std::vector<std::string> pathsToTextures;
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Assassin_ArrowStem_01.png");
	spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);
	this->arrowTail->GetTransform()->SetScale( glm::vec3(GetFloatGameConstant(GAME_CONSTANT_target_indicator_scale)));
	this->arrowTail->GetTransform()->Rotate(90.0f inRadians, XAXIS);
	this->arrowTail->SetVisible(false);

}

void Assassin::destroy() {
}

bool Assassin::isSpecialTouch(int touchId) {
	if(this->getTouchNearUnit()) {
		Touch touch = ENGINE->GetInput()->GetTouch(touchId);
		if(touch.timeDown <= GetFloatGameConstant(GAME_CONSTANT_swipe_duration_in_seconds) && glm::distance(touch.pos, this->touchStartPos) > GetFloatGameConstant(GAME_CONSTANT_swipe_dist_in_pixels)) {
			this->swipeDirectionScreen = this->touchStartPos - touch.pos;
			this->targetedCell = nullptr;
			this->SetTouchIndicatorSprite(ASSASSIN_SPECIAL);
			this->arrowTail->SetVisible(true);
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
		this->onSpecialCancelled();
	}
}

void Assassin::startSpecial() {
	PlayerUnit::startSpecial();
	this->gridNavRef->SetMovementSpeed(GetFloatGameConstant(GAME_CONSTANT_assassin_attack_speed));
	this->gridNavRef->SetDestination(this->targetedCell->x, this->targetedCell->z);
	//this->startTouchIndicatorPulse();
}

void Assassin::onSpecialEnd() {
	PlayerUnit::onSpecialEnd();
	this->arrowTail->SetVisible(false);
	this->gridNavRef->SetMovementSpeed(MOVE_SPEED);

}

void Assassin::touchedCellChanged(GridCell* prevTouchedCell) {
	if(this->inputState == InputState::INPUT_STATE_NAV) {
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
		this->gridNavRef->SetLookTarget(this->targetedCell->center);
		this->SetTouchIndicatorCell(this->targetedCell);
		this->TouchIndicatorLookAt(this->targetedCell);
		
		float dist = glm::distance(this->gridNavRef->GetCurrentCell()->center, this->targetedCell->center) - .5f;
		this->arrowTail->GetTransform()->SetScale(1.0f, dist, 1.0f);

		Transform* trans = this->arrowTail->GetComponent<Transform>();
		glm::vec3 direction = this->targetedCell->center - this->gridNavRef->GetCurrentCell()->center;
		direction = glm::normalize(direction);
		float angle = acos(glm::dot(direction, ZAXIS));
		
		if(direction.x < 0) {
			angle = -angle;
		}

		if(isnan(angle)) {
			return;
		}

		// Since the plane is normally facing the the -ZAXIS we have to do this
		trans->SetOrientation(0, YAXIS);
		this->arrowTail->GetTransform()->Rotate(90.0f inRadians, XAXIS);
		trans->Rotate(angle, YAXIS);


		trans->SetPosition(this->gridNavRef->GetCurrentCell()->center + glm::vec3(0.0f, .1f, 0.0f));
		this->arrowTail->GetTransform()->Translate(dist * .5f , trans->GetUp());
	} else {
	}
}
