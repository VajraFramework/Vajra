#include "ExampleGame/Components/GameScripts/Units/PlayerUnit.h"
#include "ExampleGame/Components/Grid/GridCell.h"
#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Libraries/glm/gtx/vector_angle.hpp"
#include "Vajra/Common/Messages/CustomMessageDatas/MessageData1S1I1F.h"
#include "Vajra/Engine/Components/DerivedComponents/Renderer/SpriteRenderer.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"

#include "Vajra/Engine/DebugDrawer/DebugDrawer.h"

#define TOUCH_SCALE_TIME .3f

void playerUnitNumberTweenCallback(float /* fromNumber */, float /* toNumber */, float currentNumber, std::string tweenClipName, MessageData1S1I1F* userParams) {
	GameObject* go = ENGINE->GetSceneGraph3D()->GetGameObjectById(userParams->i);
	//ASSERT(go != nullptr, "Game object id passed into playerUnitNuumberTweenCallback is not valid");
	if(go != nullptr) {
		PlayerUnit* pUnit = go->GetComponent<PlayerUnit>();
		ASSERT(pUnit != nullptr, "Game object passed into playerUnitNuumberTweenCallback doesn't have a player unit");
		if(pUnit != nullptr) {
			if(tweenClipName == "pulse") {
				float scaleValue = sinf(currentNumber);
				pUnit->touchIndicatorRef->GetTransform()->SetScale(scaleValue, scaleValue, scaleValue);
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
	this->inputState = InputState::INPUT_STATE_NONE;
	this->touchNearUnit = false;
	this->performingSpecial = false;
	this->unitHasTouchFocus = false;
	this->gridNavRef->SetMovementSpeed(MOVE_SPEED);
	this->gridNavRef->SetTurnSpeedDegrees(TURN_SPEED_DEG);

	this->addSubscriptionToMessageType(MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION, this->GetTypeId(), false);

	this->touchIndicatorRef = new GameObject(ENGINE->GetSceneGraph3D());
	SpriteRenderer* spriteRenderer = this->touchIndicatorRef->AddComponent<SpriteRenderer>();
	std::vector<std::string> pathsToTextures;
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Touch_Indicator_03.png");
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Touch_Fail_01.png");
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Assassin_Arrow_05.png");
	pathsToTextures.push_back(FRAMEWORK->GetFileSystemUtils()->GetDevicePictureResourcesFolderName() + "SD_UIEffect_Thief_Jump_cyan.png");
	
	spriteRenderer->initPlane(1.0f, 1.0f, "sptshdr", pathsToTextures, PlaneOrigin::Center);

	this->touchIndicatorRef->SetVisible(false);
	this->touchIndicatorRef->GetTransform()->Rotate(90.0f inRadians, XAXIS);

	this->currentTouchedCell = NULL;


	this->SwitchActionState(UNIT_ACTION_STATE_IDLE);
}

void PlayerUnit::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}

void PlayerUnit::HandleMessage(MessageChunk messageChunk) {
	BaseUnit::HandleMessage(messageChunk);
	switch(messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION:
			if(this->performingSpecial) {
				this->onSpecialEnd();
			}  else if(this->inputState == InputState::INPUT_STATE_WAIT || this->inputState == InputState::INPUT_STATE_NONE) {
				this->SwitchActionState(UNIT_ACTION_STATE_IDLE);
				ENGINE->GetTween()->CancelNumberTween("pulse");
				ENGINE->GetTween()->TweenScale(this->touchIndicatorRef->GetId(), this->touchIndicatorRef->GetTransform()->GetScale(), glm::vec3(0), TOUCH_SCALE_TIME);
			}
			break;
		default:
			break;
	}
}

void PlayerUnit::OnTouch(int touchId, GridCell* touchedCell) {
	bool touchBegan = ENGINE->GetInput()->GetTouch(touchId).phase == TouchPhase::Began;
	if(touchBegan) {
		this->unitHasTouchFocus = true;
	}

	if(this->performingSpecial) {
		this->unitHasTouchFocus = false;
		return;
	}
	// only handle a touch that was started when the unit has focus
	if(this->unitHasTouchFocus) {
		if(this->currentTouchedCell != touchedCell || touchBegan) {
			GridCell* prevTouchedCell = this->currentTouchedCell;
			this->currentTouchedCell = touchedCell;
			this->touchedCellChanged(prevTouchedCell);
		}
		
		if(touchBegan) {
			this->touchStartPos = ENGINE->GetInput()->GetTouch(touchId).pos;
			this->setTouchNearUnit();
		}	

		switch(this->inputState) {
			case InputState::INPUT_STATE_NONE:
				this->onSelectedTouch();
			case InputState::INPUT_STATE_WAIT:
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
}

void PlayerUnit::OnDeselect() {
	this->inputState = InputState::INPUT_STATE_NONE;
	ENGINE->GetTween()->CancelScaleTween(this->touchIndicatorRef->GetId());
	ENGINE->GetTween()->CancelNumberTween("pulse");
	this->SetTouchIndicatorVisible(false);
				
}

void PlayerUnit::OnTransitionZoneEntered(GridCell* newTarget) {
	if(this->performingSpecial) {
		this->cancelSpecial();
	}
	this->SetTouchIndicatorCell(newTarget);
	this->SwitchActionState(UNIT_ACTION_STATE_WALKING);
	this->startTouchIndicatorPulse();
	this->gridNavRef->SetDestination(newTarget);
	this->touchIndicatorRef->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(GOOD_TOUCH);
	this->touchIndicatorRef->SetVisible(true);
}

void PlayerUnit::onSelectedTouch() {
	this->inputState = InputState::INPUT_STATE_WAIT;
}

void PlayerUnit::startSpecial() {
	this->performingSpecial = true;
	this->SwitchActionState(UNIT_ACTION_STATE_DOING_SPECIAL);
	this->specialStartPos = this->GetObject()->GetComponent<Transform>()->GetPositionWorld();
}

void PlayerUnit::onSpecialEnd() {
	this->performingSpecial = false;
	this->inputState = InputState::INPUT_STATE_NONE;
	this->SwitchActionState(UNIT_ACTION_STATE_POST_SPECIAL);
	this->touchIndicatorRef->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(GOOD_TOUCH);
	this->touchIndicatorRef->SetVisible(false);
}

void PlayerUnit::cancelSpecial() {
	this->performingSpecial = false;
	this->inputState = InputState::INPUT_STATE_NONE;
	this->SwitchActionState(UNIT_ACTION_STATE_IDLE);
	this->touchIndicatorRef->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(GOOD_TOUCH);
	this->touchIndicatorRef->SetVisible(false);
}

void PlayerUnit::onNavTouch(int touchId, GridCell* touchedCell) {
	
	if(this->isSpecialTouch(touchId)) {
		this->inputState = InputState::INPUT_STATE_SPECIAL;
		this->gridNavRef->StopNavigation();
		this->onSpecialTouch(touchId);
		this->SwitchActionState(UNIT_ACTION_STATE_PRE_SPECIAL);
	} else {
		switch(ENGINE->GetInput()->GetTouch(touchId).phase) {
			case TouchPhase::Began:
				this->inputState = InputState::INPUT_STATE_NAV;
				this->touchIndicatorRef->GetTransform()->SetPosition(this->currentTouchedCell->center);
				this->touchIndicatorRef->GetTransform()->Translate(0.01f, YAXIS);
				// touch indicator tween up
				this->touchIndicatorRef->SetVisible(true);
				ENGINE->GetTween()->CancelScaleTween(this->touchIndicatorRef->GetId());
				ENGINE->GetTween()->CancelNumberTween("pulse");
				ENGINE->GetTween()->TweenScale(this->touchIndicatorRef->GetId(), glm::vec3(0), glm::vec3(1),TOUCH_SCALE_TIME);
				break;
			case TouchPhase::Ended:
				this->currentTouchedCell = nullptr;
				this->inputState = InputState::INPUT_STATE_WAIT;
				if(this->gridNavRef->SetDestination(touchedCell->x, touchedCell->z) && touchedCell != this->gridNavRef->GetCurrentCell()) {
					this->SwitchActionState(UNIT_ACTION_STATE_WALKING);
					this->startTouchIndicatorPulse();
				} else {
					ENGINE->GetTween()->CancelScaleTween(this->touchIndicatorRef->GetId());
					ENGINE->GetTween()->CancelNumberTween("pulse");
					ENGINE->GetTween()->TweenScale(this->touchIndicatorRef->GetId(), glm::vec3(1), glm::vec3(0), TOUCH_SCALE_TIME * 2.0f);
				}
				if(touchedCell == this->gridNavRef->GetCurrentCell()) {
					ENGINE->GetTween()->CancelNumberTween("pulse");
					ENGINE->GetTween()->TweenScale(this->touchIndicatorRef->GetId(), this->touchIndicatorRef->GetTransform()->GetScale(), glm::vec3(0), TOUCH_SCALE_TIME);
				}
				break;
			case TouchPhase::Cancelled:
				this->inputState = InputState::INPUT_STATE_WAIT;
				this->currentTouchedCell = nullptr;
				this->touchIndicatorRef->SetVisible(false);
				break;
			default:
				break;
		}
	}
}

void PlayerUnit::touchedCellChanged(GridCell* /*prevTouchedCell*/) {
	this->SetTouchIndicatorCell(this->currentTouchedCell);
	if(this->inputState == InputState::INPUT_STATE_NAV || this->inputState == InputState::INPUT_STATE_WAIT) {
		if(this->gridNavRef->CanReachDestination(this->currentTouchedCell)) {
			this->touchIndicatorRef->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(GOOD_TOUCH);
		} else {
			this->touchIndicatorRef->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(BAD_TOUCH);
		}
		// Toggle the touch indicator visibility 
		bool standable;
		for(int i = 0; i < NUM_ELEVATIONS; ++i) {
			standable = SINGLETONS->GetGridManager()->GetGrid()->IsCellPassableAtElevation(this->currentTouchedCell->x,
																						   this->currentTouchedCell->z,
																						   i);
			if(standable) {
				break;
			}
		}
		this->touchIndicatorRef->SetVisible(standable);
	}
}

void PlayerUnit::TouchIndicatorLookAt(GridCell* target) {
	this->GridPlaneLookAt(this->touchIndicatorRef, target);
}

void PlayerUnit::GridPlaneLookAt(GameObject* plane, GridCell* target) {
	Transform* trans = plane->GetComponent<Transform>();
	glm::vec3 direction = target->center - this->gridNavRef->GetCurrentCell()->center;
	direction = glm::normalize(direction);
	float angle = acos(glm::dot(direction, ZAXIS));
	
	if(direction.x < 0) {
		angle = -angle;
	}

	if(glm::isnan(angle)) {
		return;
	}

	// Since the plane is normally facing the the -ZAXIS we have to do this
	trans->SetOrientation(0, YAXIS);
	trans->Rotate(90.0f inRadians, XAXIS);
	trans->Rotate(angle, YAXIS);
}
void PlayerUnit::setTouchNearUnit() {
	glm::vec3 gridPos = SINGLETONS->GetGridManager()->TouchPositionToGridPosition(touchStartPos);
	if(glm::distance(gridPos, this->gridNavRef->GetCurrentCell()->center) < NEAR_TOUCH_DIST) {
		this->touchNearUnit = true;
	} else {
		this->touchNearUnit = false;
	}
}

void PlayerUnit::startTouchIndicatorPulse() {
	MessageData1S1I1F* userParams = new MessageData1S1I1F();
	userParams->i = this->GetObject()->GetId();
	ENGINE->GetTween()->CancelScaleTween(this->touchIndicatorRef->GetId());
	ENGINE->GetTween()->TweenToNumber(45.0f inRadians, 135.0f inRadians, 1.0f, true, true, true, "pulse", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D, userParams, playerUnitNumberTweenCallback);
}


void PlayerUnit::SetTouchIndicatorSprite(int index) {
	this->touchIndicatorRef->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(index);
}

void PlayerUnit::SetTouchIndicatorCell(GridCell* c) {
	this->touchIndicatorRef->GetTransform()->SetPosition(c->center);
	this->touchIndicatorRef->GetTransform()->Translate(0.01f + (c->y * 0.05), YAXIS);
}

void PlayerUnit::SetTouchIndicatorVisible(bool visibility) {
	this->touchIndicatorRef->SetVisible(visibility);
}
