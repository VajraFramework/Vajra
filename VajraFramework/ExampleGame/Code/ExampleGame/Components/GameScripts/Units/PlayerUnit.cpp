#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1I1F.h"
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

#define TOUCH_SCALE_TIME .3f

void playerUnitNumberTweenCallback(float /* fromNumber */, float /* toNumber */, float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams) {
	GameObject* go = ENGINE->GetSceneGraph3D()->GetGameObjectById(userParams->i);
	ASSERT(go != nullptr, "Game object id passed into playerUnitNuumberTweenCallback is not valid");
	if(go != nullptr) {
		PlayerUnit* pUnit = go->GetComponent<PlayerUnit>();
		ASSERT(pUnit != nullptr, "Game object passed into playerUnitNuumberTweenCallback doesn't have a player unit");
		if(pUnit != nullptr) {
			if(tweenClipName == "pulse") {
				float scaleValue = sinf(currentNumber);
				pUnit->touchIndicator->GetTransform()->SetScale(scaleValue, scaleValue, scaleValue);
			}
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

	this->gridNavRef->SetMovementSpeed(MOVE_SPEED);
	this->gridNavRef->SetTurnSpeedDegrees(TURN_SPEED_DEG);

	this->addSubscriptionToMessageType(MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION, this->GetTypeId(), false);

	this->touchIndicator = new GameObject(ENGINE->GetSceneGraph3D());
	SpriteRenderer* spriteRenderer = this->touchIndicator->AddComponent<SpriteRenderer>();
	std::vector<std::string> pathsToTextures;
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_Touch_Good.png");
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_Touch_Bad.png");
	spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);

	touchIndicator->SetVisible(false);
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
			}  else {
				ENGINE->GetTween()->CancelNumberTween("pulse");
				ENGINE->GetTween()->TweenScale(this->touchIndicator->GetId(), this->touchIndicator->GetTransform()->GetScale(), glm::vec3(0), TOUCH_SCALE_TIME);
			}
			break;
		default:
			break;
	}
}

void PlayerUnit::OnTouch(int touchId, GridCell* touchedCell) {
	if(this->currentTouchedCell != touchedCell || ENGINE->GetInput()->GetTouch(touchId).phase == TouchPhase::Began) {
		this->currentTouchedCell = touchedCell;
		this->touchedCellChanged();
	}
	
	if(ENGINE->GetInput()->GetTouch(touchId).phase == TouchPhase::Began) {
		this->touchStartPos = ENGINE->GetInput()->GetTouch(touchId).pos;
		this->setTouchNearUnit();
		this->touchIndicator->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(GOOD_TOUCH);
	}	

	if(this->performingSpecial) {
		return;
	}

	switch(this->inputState) {
		case InputState::INPUT_STATE_WAIT:
			this->onSelectedTouch();
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
	this->inputState = InputState::INPUT_STATE_WAIT;
	touchIndicator->SetVisible(false);
}
void PlayerUnit::onNavTouch(int touchId, GridCell* touchedCell) {
	
	if(this->isSpecialTouch(touchId)) {
		this->inputState = InputState::INPUT_STATE_SPECIAL;
		this->gridNavRef->StopNavigation();
		this->touchIndicator->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(BAD_TOUCH);
		this->onSpecialTouch(touchId);
	} else {
		MessageData1S1I1F* userParams;
		switch(ENGINE->GetInput()->GetTouch(touchId).phase) {
			case TouchPhase::Began:
				touchIndicator->GetTransform()->SetPosition(this->currentTouchedCell->center);
				touchIndicator->GetTransform()->Translate(0.01f, YAXIS);
				touchIndicator->SetVisible(true);
				// touch indicator tween up
				ENGINE->GetTween()->CancelScaleTween(this->touchIndicator->GetId());
				ENGINE->GetTween()->CancelNumberTween("pulse");
				ENGINE->GetTween()->TweenScale(this->touchIndicator->GetId(), glm::vec3(0), glm::vec3(1),TOUCH_SCALE_TIME);
				break;
			case TouchPhase::Ended:
				this->currentTouchedCell = nullptr;
				this->gridNavRef->SetDestination(touchedCell->x, touchedCell->z);
				userParams = new MessageData1S1I1F();
				userParams->i = this->GetObject()->GetId();
				ENGINE->GetTween()->CancelScaleTween(this->touchIndicator->GetId());
				ENGINE->GetTween()->TweenToNumber(45.0f inRadians, 135.0f inRadians, 1.0f, true, true, true, "pulse", userParams, playerUnitNumberTweenCallback);
				break;
			case TouchPhase::Cancelled:
				this->currentTouchedCell = nullptr;
				touchIndicator->SetVisible(false);
				break;
			default:
				break;
		}
	}
}

void PlayerUnit::touchedCellChanged() {
	this->touchIndicator->GetTransform()->SetPosition(this->currentTouchedCell->center);
	this->touchIndicator->GetTransform()->Translate(0.01f, YAXIS);
	if(this->inputState == InputState::INPUT_STATE_NAV) {
		if(this->gridNavRef->CanReachDestination(this->currentTouchedCell)) {
			this->touchIndicator->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(GOOD_TOUCH);
		} else {
			this->touchIndicator->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(BAD_TOUCH);
		}
	}
}

void PlayerUnit::setTouchNearUnit() {
	glm::vec3 gridPos = SINGLETONS->GetGridManager()->TouchPositionToGridPosition(touchStartPos);
	if(glm::distance(gridPos, this->gameObjectRef->GetTransform()->GetPosition()) < NEAR_TOUCH_DIST) {
		this->touchNearUnit = true;
	} else {
		this->touchNearUnit = false;
	}
}
