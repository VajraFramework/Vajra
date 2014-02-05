#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"

#define GOOD_TOUCH 0
#define BAD_TOUCH  1

static GameObject* s_touchIndicator;

void tweenNumberCallback(float fromNumber, float toNumber, float currentNumber, std::string tweenClipName) {
	if(tweenClipName == "scale") {
		s_touchIndicator->GetTransform()->SetScale(currentNumber, currentNumber, currentNumber);
	} else if(tweenClipName == "pulse") {
		float scaleValue = sinf(currentNumber);

		s_touchIndicator->GetTransform()->SetScale(scaleValue, scaleValue, scaleValue);
	}
}

PlayerUnit::PlayerUnit() : BaseUnit() {
	this->init();
}

PlayerUnit::PlayerUnit(Object* object_) : BaseUnit(object_) {
	this->init();
}

PlayerUnit::~PlayerUnit() {
	this->destroy();
}

void PlayerUnit::init() {
	this->unitType = UnitType::UNIT_TYPE_ASSASSIN;
	this->inputState = InputState::INPUT_STATE_WAIT;

	// DECAL TEST
	this->touchIndicator = new GameObject(ENGINE->GetSceneGraph3D());
	s_touchIndicator = this->touchIndicator;
	SpriteRenderer* spriteRenderer = this->touchIndicator->AddComponent<SpriteRenderer>();
	touchIndicator->SetVisible(false);
	std::vector<std::string> pathsToTextures;
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_Touch_Good.png");
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_Touch_Bad.png");
	spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);
	//
	this->touchIndicator->GetTransform()->Rotate(90.0f inRadians, XAXIS);
	
	this->currentTouchedCell = NULL;
}

void PlayerUnit::destroy() {
}

void PlayerUnit::OnTouch(int touchId, GridCell* touchedCell) {
	if(this->currentTouchedCell != touchedCell) {
		this->currentTouchedCell = touchedCell;
		this->touchedCellChanged();
	}
	
	switch(this->inputState) {
		case InputState::INPUT_STATE_WAIT:
			this->onSelectedTouch();
			break;
		case InputState::INPUT_STATE_NAV:
			this->onNavTouch(touchId, touchedCell);
			break;
		case InputState::INPUT_STATE_SPECIAL:
			this->onSpecialTouch(touchId);
			break;
		default:
			break;
	}
}

void PlayerUnit::OnDeselect() {
	this->inputState = InputState::INPUT_STATE_WAIT;
}
void PlayerUnit::onSelectedTouch() {
	this->inputState = InputState::INPUT_STATE_NAV;
}

void PlayerUnit::onNavTouch(int touchId, GridCell* touchedCell) {
	
	if(this->isSpecialTouch(touchId)) {

	}
	else {
		switch(ENGINE->GetInput()->GetTouch(touchId).phase) {
			case TouchPhase::Began:
				touchIndicator->GetTransform()->SetPosition(this->currentTouchedCell->center);
				touchIndicator->SetVisible(true);
				// touch indicator tween up
				ENGINE->GetTween()->TweenToNumber(.3f, 1.0f, .3f, true, false, true, "scale", tweenNumberCallback);
				break;
			case TouchPhase::Ended:
				break;
			case TouchPhase::Cancelled:
				touchIndicator->SetVisible(false);
				break;
			default:
				break;
		}
		if(ENGINE->GetInput()->GetTouch(touchId).phase == TouchPhase::Ended) {
			this->gridNavRef->SetDestination(touchedCell->x, touchedCell->z);
			//ENGINE->GetTween()->TweenToNumber(0.0f inRadians, 180.0f inRadians, 2.0f, true, true, true, "pulse", tweenNumberCallback);
		}
	}

}

void PlayerUnit::touchedCellChanged() {
	
	this->touchIndicator->GetTransform()->SetPosition(this->currentTouchedCell->center);
	/*if(this->gridNavRef->SetDestination(touchedCell->x, touchedCell->z)) {
		this->touchIndicator->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(GOOD_TOUCH);
	} else {
		this->touchIndicator->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(BAD_TOUCH);
	}*/
}