#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
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

namespace PlayerUnitTween {
	void tweenNumberCallback(float fromNumber, float toNumber, float currentNumber, std::string tweenClipName) {
		if(tweenClipName == "scale") {
			s_touchIndicator->GetTransform()->SetScale(currentNumber, currentNumber, currentNumber);
		} else if(tweenClipName == "pulse") {
			float scaleValue = sinf(currentNumber);

			s_touchIndicator->GetTransform()->SetScale(scaleValue, scaleValue, scaleValue);
		}
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
	this->touchNearUnit = false;
	this->performingSpecial = false;

	this->moveSpeed = 2.5f;
	this->turnSpeedDegrees = 360.0f;
	this->gridNavRef->SetMovementSpeed(this->moveSpeed);
	this->gridNavRef->SetTurnSpeedDegrees(this->turnSpeedDegrees);

	this->addSubscriptionToMessageType(MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION, this->GetTypeId(), false);
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
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void PlayerUnit::HandleMessage(MessageChunk messageChunk) {
	BaseUnit::HandleMessage(messageChunk);
	switch(messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION:
			if(this->performingSpecial) {
				onSpecialEnd();
			}
			break;
		default:
			break;
	}
}

void PlayerUnit::OnTouch(int touchId, GridCell* touchedCell) {
	if(this->currentTouchedCell != touchedCell) {
		this->currentTouchedCell = touchedCell;
		this->touchedCellChanged();
	}
	
	if(ENGINE->GetInput()->GetTouch(touchId).phase == TouchPhase::Began) {
		this->touchStartPos = ENGINE->GetInput()->GetTouch(touchId).pos;
		this->setTouchNearUnit();
	}	

	if(this->performingSpecial) {
		return;
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

void PlayerUnit::startSpecial() {
	this->performingSpecial = true;
}

void PlayerUnit::onSpecialEnd() {
	this->performingSpecial = false;
	this->inputState = InputState::INPUT_STATE_NAV;
}
void PlayerUnit::onNavTouch(int touchId, GridCell* touchedCell) {
	
	if(this->isSpecialTouch(touchId)) {
		this->inputState = InputState::INPUT_STATE_SPECIAL;
		this->gridNavRef->StopNavigation();

	}
	else {
		switch(ENGINE->GetInput()->GetTouch(touchId).phase) {
			case TouchPhase::Began:
				touchIndicator->GetTransform()->SetPosition(this->currentTouchedCell->center);
				touchIndicator->SetVisible(true);
				// touch indicator tween up
				ENGINE->GetTween()->TweenScale(this->touchIndicator->GetId(), glm::vec3(0), glm::vec3(1), .3f);
				ENGINE->GetTween()->CancelNumberTween("pulse");
				break;
			case TouchPhase::Ended:
				this->gridNavRef->SetDestination(touchedCell->x, touchedCell->z);
				ENGINE->GetTween()->CancelScaleTween(this->touchIndicator->GetId());
				ENGINE->GetTween()->TweenToNumber(45.0f inRadians, 135.0f inRadians, 1.0f, false, true, true, "pulse", PlayerUnitTween::tweenNumberCallback);
				break;
			case TouchPhase::Cancelled:
				touchIndicator->SetVisible(false);
				break;
			default:
				break;
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

void PlayerUnit::setTouchNearUnit() {
	glm::vec3 gridPos = SINGLETONS->GetGridManager()->TouchPositionToGridPosition(touchStartPos);
	if(glm::distance(gridPos, this->gameObjectRef->GetTransform()->GetPosition()) < nearTouchDist) {
		this->touchNearUnit = true;
	} else {
		this->touchNearUnit = false;
	}
}
