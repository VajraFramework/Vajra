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
	this->unitHasTouchFocus = false;
	this->gridNavRef->SetMovementSpeed(MOVE_SPEED);
	this->gridNavRef->SetTurnSpeedDegrees(TURN_SPEED_DEG);
	this->gridNavRef->SetMaxNavigableUnitType(LAST_PLAYER_UNIT_TYPE);

	this->addSubscriptionToMessageType(MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION, this->GetTypeId(), false);

	this->touchIndicatorRef = new GameObject(ENGINE->GetSceneGraph3D());
	SpriteRenderer* spriteRenderer = this->touchIndicatorRef->AddComponent<SpriteRenderer>();
	spriteRenderer->SetHasTransperancy(true);
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
			if(this->GetUnitActionState() == UNIT_ACTION_STATE_DOING_SPECIAL) {
				this->onSpecialEnd();
			}  else if(this->GetUnitActionState() != UNIT_ACTION_STATE_PRE_SPECIAL) {
				this->SwitchActionState(UNIT_ACTION_STATE_IDLE);
				if(this->inputState == InputState::INPUT_STATE_WAIT || this->inputState == InputState::INPUT_STATE_NONE) {
					ENGINE->GetTween()->CancelNumberTween("pulse");
					ENGINE->GetTween()->TweenScale(this->touchIndicatorRef->GetId(), this->touchIndicatorRef->GetTransform()->GetScale(), glm::vec3(0), TOUCH_SCALE_TIME);
				}
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
	
	if(this->GetUnitActionState() == UnitActionState::UNIT_ACTION_STATE_DOING_SPECIAL ||
	   this->GetUnitActionState() == UnitActionState::UNIT_ACTION_STATE_POST_SPECIAL) {
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

		if(this->inputState == InputState::INPUT_STATE_NONE) {
			this->onSelectedTouch();
		}
		switch(this->inputState) {
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
	if(this->GetUnitActionState() == UNIT_ACTION_STATE_DOING_SPECIAL || this->GetUnitActionState() == UNIT_ACTION_STATE_POST_SPECIAL) {
		this->cancelSpecial();
	}
	this->SetTouchIndicatorLocation(newTarget);
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
	this->SwitchActionState(UNIT_ACTION_STATE_DOING_SPECIAL);
	this->specialStartPos = this->GetObject()->GetComponent<Transform>()->GetPositionWorld();
}

void PlayerUnit::onSpecialEnd() {
	this->inputState = InputState::INPUT_STATE_WAIT;
	this->SwitchActionState(UNIT_ACTION_STATE_POST_SPECIAL);
	this->touchIndicatorRef->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(GOOD_TOUCH);
	this->touchIndicatorRef->SetVisible(false);
	this->touchStartPos = glm::vec2();
	this->touchNearUnit = false;
}

void PlayerUnit::cancelSpecial() {
	this->inputState = InputState::INPUT_STATE_WAIT;
	this->SwitchActionState(UNIT_ACTION_STATE_IDLE);
	this->touchIndicatorRef->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(GOOD_TOUCH);
	this->touchIndicatorRef->SetVisible(false);
	this->touchStartPos = glm::vec2();
	this->touchNearUnit = false;
}

void PlayerUnit::onNavTouch(int touchId, GridCell* touchedCell) {
	if(this->isSpecialTouch(touchId)) {
		this->inputState = InputState::INPUT_STATE_SPECIAL; 
		this->onSpecialTouch(touchId);
		this->SwitchActionState(UNIT_ACTION_STATE_PRE_SPECIAL);
	} else {
		switch(ENGINE->GetInput()->GetTouch(touchId).phase) {
			case TouchPhase::Began:
				this->inputState = InputState::INPUT_STATE_NAV;
				this->SetTouchIndicatorLocation(this->currentTouchedCell);
				ENGINE->GetTween()->CancelScaleTween(this->touchIndicatorRef->GetId());
				ENGINE->GetTween()->CancelNumberTween("pulse");
				ENGINE->GetTween()->TweenScale(this->touchIndicatorRef->GetId(), glm::vec3(0), glm::vec3(1),TOUCH_SCALE_TIME);
				break;
			case TouchPhase::Ended:
				this->inputState = InputState::INPUT_STATE_WAIT;
				if(this->currentTouchedCell != nullptr && this->gridNavRef->SetDestination(this->currentTouchedCell->x, this->currentTouchedCell->z) && this->currentTouchedCell != this->gridNavRef->GetCurrentCell()) {
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

				this->currentTouchedCell = nullptr;
				break;
			case TouchPhase::Cancelled:
				this->inputState = InputState::INPUT_STATE_WAIT;
				this->currentTouchedCell = nullptr;
				this->touchIndicatorRef->SetVisible(false);
				ENGINE->GetTween()->CancelScaleTween(this->touchIndicatorRef->GetId());
				ENGINE->GetTween()->CancelNumberTween("pulse");
				break;
			default:
				break;
		}
	}
}

void PlayerUnit::touchedCellChanged(GridCell* /*prevTouchedCell*/) {
	this->SetTouchIndicatorLocation(this->currentTouchedCell);
	if(this->inputState == InputState::INPUT_STATE_NAV || this->inputState == InputState::INPUT_STATE_WAIT) {
		if(this->gridNavRef->CanReachDestination(this->currentTouchedCell)) {
			this->touchIndicatorRef->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(GOOD_TOUCH);
		} else {
			this->touchIndicatorRef->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(BAD_TOUCH);
		}
		if(this->currentTouchedCell != nullptr) {
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
		} else {
			this->touchIndicatorRef->SetVisible(false);
		}
	}
}

void PlayerUnit::TouchIndicatorLookAt(GridCell* target) {
	this->GridPlaneLookAt(this->touchIndicatorRef, target->center);
}

void PlayerUnit::GridPlaneLookAt(GameObject* plane, GridCell* target) {
	this->GridPlaneLookAt(plane, target->center);
}

void PlayerUnit::GridPlaneLookAt(GameObject* plane, glm::vec3 target) {
	Transform* trans = plane->GetComponent<Transform>();
	glm::vec3 direction = target - this->gameObjectRef->GetTransform()->GetPosition();
	direction = glm::normalize(direction);
	float angle = acos(glm::dot(direction, ZAXIS));
	
	if(direction.x < 0) {
		angle = -angle;
	}

	if(std::isnan(angle)) {
		return;
	}

	// Since the plane is normally facing the the -ZAXIS we have to do this
	trans->SetOrientation(0, YAXIS);
	trans->Rotate(90.0f inRadians, XAXIS);
	trans->Rotate(angle, YAXIS);
}
void PlayerUnit::setTouchNearUnit() {
	glm::vec3 gridPos = SINGLETONS->GetGridManager()->TouchPositionToGridPositionAtElevation(touchStartPos, this->gridNavRef->GetCurrentCell()->y);
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
	ENGINE->GetTween()->TweenToNumber(45.0f inRadians, 135.0f inRadians, 1.0f, INTERPOLATION_TYPE_LINEAR, true, true, true, "pulse", NUMBER_TWEEN_AFFILIATION_SCENEGRAPH_3D, userParams, playerUnitNumberTweenCallback);
}


void PlayerUnit::SetTouchIndicatorSprite(int index) {
	this->touchIndicatorRef->GetComponent<SpriteRenderer>()->SetCurrentTextureIndex(index);
}

void PlayerUnit::SetTouchIndicatorLocation(GridCell* c) {
	if(c != nullptr) {
		glm::vec3 target = c->center;
		target.y += c->y * 0.05;
		this->SetTouchIndicatorLocation(target);
	}
}

void PlayerUnit::SetTouchIndicatorLocation(glm::vec3 target) {
	this->touchIndicatorRef->GetTransform()->SetPosition(target);
	this->touchIndicatorRef->GetTransform()->Translate(0.05f , YAXIS);

}

void PlayerUnit::SetTouchIndicatorVisible(bool visibility) {
	this->touchIndicatorRef->SetVisible(visibility);
}
