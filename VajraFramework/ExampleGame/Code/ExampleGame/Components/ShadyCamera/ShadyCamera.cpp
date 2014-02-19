#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/Grid/GridRoom.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
#include "ExampleGame/GameConstants/GameConstants.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "ExampleGame/Messages/Declarations.h"
#include "Vajra/Common/Messages/Message.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Engine/MessageHub/MessageHub.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"
#include "Vajra/Engine/Tween/Tween.h"
#include "Vajra/Utilities/MathUtilities.h"

namespace ShadyCameraTween {
	void tweenCallback(ObjectIdType /* gameObjectId */, std::string /* tweenClipName */) {
	}
}

ShadyCamera::ShadyCamera() : Camera() {
	this->init();
}

ShadyCamera::ShadyCamera(Object* object_) : Camera(object_) {
	this->init();
}

ShadyCamera::~ShadyCamera() {
	this->destroy();
}

void ShadyCamera::init() {
	// Set camera properties
	this->SetFOV(30.0f inRadians);
	this->camSpeed = GetFloatGameConstant(GAME_CONSTANT_camSpeed);
	this->camMode = CameraMode::CameraMode_Game;
	
	this->newPinch = false;
	this->velocityThreshold = 7.0f;
	this->heightThreshold = 10.0f;

	this->gameObjectRef = (GameObject*)this->GetObject();
	Transform* camTransform = this->gameObjectRef->GetTransform();

	// TODO [Implement] : set this based on device resolution
	this->gameCamOffset = glm::vec3(0.0f, 26.0f, 0.0f);

	// TODO [Implement] : get overview pos and start room from level data
	this->overviewPos = glm::vec3(0.0f, 50.0f, 0.0f);
	this->gameCamPos = this->gameCamOffset;

	camTransform->SetPosition(this->gameCamPos);
	camTransform->Rotate(90.0f inRadians, XAXIS);
	camTransform->Rotate(180.0f inRadians, camTransform->GetForward());

	this->gridManagerRef = nullptr;

}

void ShadyCamera::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
	this->gameObjectRef = nullptr;
	this->gridManagerRef = nullptr;
}

void ShadyCamera::loadCameraData(GridCell* startUnitCell, glm::vec3 overviewPos, glm::vec3 startPos, bool useStartPos) {

	this->overviewPos = overviewPos;
	Transform* camTransform = this->gameObjectRef->GetTransform();
	if(useStartPos) {
		camTransform->SetPosition(startPos);
		this->MoveGameCamToRoom(startUnitCell->x, startUnitCell->z);
	} else {
		this->setCurrentRoomCenter(this->gridManagerRef->GetGrid()->GetRoomCenter(startUnitCell->x, startUnitCell->z));
		this->gameCamPos = this->currentRoomCenter + this->gameCamOffset;		
		camTransform->SetPosition(gameCamPos);

	}
	// We only want to subscribe to messages once we set up the inital camera data 
	this->addSubscriptionToMessageType(MESSAGE_TYPE_PINCH_GESTURE, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SELECTED_UNIT_CHANGED, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ROOM_ENTERED, this->GetTypeId(), false);
}
void ShadyCamera::HandleMessage(MessageChunk messageChunk) {
	Camera::HandleMessage(messageChunk);
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_PINCH_GESTURE:
			this->onPinch();
			break;
		case MESSAGE_TYPE_SELECTED_UNIT_CHANGED:
			this->MoveGameCamToRoom(messageChunk->messageData.iv1.x, messageChunk->messageData.iv1.z);
			break;
		case MESSAGE_TYPE_GRID_ROOM_ENTERED:
			if(messageChunk->messageData.iv1.x == this->gridManagerRef->GetSelectedUnitId()) {
				this->MoveGameCamToRoom(messageChunk->messageData.fv1);
			}
			break;
		default:
			break;
	}
}

void ShadyCamera::SetGridManager(GridManager* newManager) {
	this->gridManagerRef = newManager;
}

void ShadyCamera::MoveTo(glm::vec3 newPos) {
	glm::vec3 curPos = this->gameObjectRef->GetTransform()->GetPosition();
	float dist = glm::distance(curPos, newPos);
	if (dist > ROUNDING_ERROR) {
		ENGINE->GetTween()->TweenPosition(this->gameObjectRef->GetId(), curPos, newPos,
				dist / this->camSpeed, true, TWEEN_TRANSLATION_CURVE_TYPE_LINEAR, false, ShadyCameraTween::tweenCallback);
	}
}

void ShadyCamera::MoveTo(float x, float y, float z) {
	this->MoveTo(glm::vec3(x, y, z));
}

void ShadyCamera::PanTo(float x, float z) {
	float y = this->gameObjectRef->GetTransform()->GetPosition().y;
	this->MoveTo(glm::vec3(x, y, z));
}

void ShadyCamera::ZoomTo(float y) {
	glm::vec3 newPos = this->gameObjectRef->GetTransform()->GetPosition();
	newPos.y = y;
	this->gameObjectRef->GetTransform()->SetPosition(newPos);
}

void ShadyCamera::ZoomBy(float yOffset) {
	glm::vec3 newPos = this->gameObjectRef->GetTransform()->GetPosition();
	newPos.y += yOffset;
	this->gameObjectRef->GetTransform()->SetPosition(newPos);
}

void ShadyCamera::MoveToGamePos() {
	this->MoveTo(this->gameCamPos);
}

void ShadyCamera::MoveToOverview() {
	this->MoveTo(this->overviewPos);
}

// TODO [Implement] : if needed
void ShadyCamera::LevelStartPan() {

}

void ShadyCamera::MoveGameCamToRoom(int i, int j) {
	this->MoveGameCamToRoom(this->gridManagerRef->GetGrid()->GetRoomCenter(i, j));
}

void ShadyCamera::MoveGameCamToRoom(glm::vec3 roomCenter) {
	this->setCurrentRoomCenter(roomCenter);
	this->updateGameCamPos();
}

void ShadyCamera::setCurrentRoomCenter(glm::vec3 roomCenter) {
	if(roomCenter != ZERO_VEC3) {
		this->currentRoomCenter = roomCenter;
	}
}
void ShadyCamera::updateGameCamPos() {
	glm::vec3 newPos = this->currentRoomCenter + this->gameCamOffset;
	if(newPos != this->gameCamPos) {
		this->gameCamPos = newPos;
		if(this->camMode == CameraMode::CameraMode_Game) {
			this->MoveToGamePos();
		}
	}
}

void ShadyCamera::onPinch() {
	if(ENGINE->GetInput()->GetPinch().gestureState == GestureState::GestureState_Start) {
		this->newPinch = true;
	}
	if(!this->newPinch) {
		return;
	}

	float pinchVel = ENGINE->GetInput()->GetPinch().velocity;
	float zoomAmt = -pinchVel;

	this->ZoomBy(zoomAmt);

	// if a mode switch occurs early out
	if(tryModeSwitch(pinchVel)) {
		this->newPinch = false;
		return;
	}

	// if a mode switch hasn't occur reset the camera
	if(ENGINE->GetInput()->HasPinchEnded()) {
		if(this->camMode == CameraMode::CameraMode_Game ) {
			this->MoveToGamePos();
		}
		else {
			this->MoveToOverview();
		}
	}
}

bool ShadyCamera::tryModeSwitch(float velocity) {
	float startY = this->camMode == CameraMode::CameraMode_Game ? this->gameCamPos.y : this->overviewPos.y;
	float camY = this->gameObjectRef->GetTransform()->GetPosition().y;
	if(std::abs(velocity) >= this->velocityThreshold || std::abs(startY - camY) >= this->heightThreshold)
	{
		// Zoom in pinch
		if(velocity > 0) {
			this->setCameraMode(CameraMode::CameraMode_Game);
			this->MoveToGamePos();
		}
		else {
			this->setCameraMode(CameraMode::CameraMode_Overview);
			this->MoveToOverview();
		}
		return true;
	} 
	return false;
}

void ShadyCamera::setCameraMode(CameraMode newMode) {
	if(newMode != this->camMode) {
		this->camMode = newMode;

		ENGINE->GetMessageHub()->SendMulticastMessage(MESSAGE_TYPE_CAMERA_MODE_CHANGED, this->gameObjectRef->GetId());
	}
}

void ShadyCamera::onUnitChanged() {
	//this->MoveToRoom();
}
