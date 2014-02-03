#include "ExampleGame/Components/Grid/GridManager.h"
#include "ExampleGame/Components/Grid/GridNavigator.h"
#include "ExampleGame/Components/Grid/GridRoom.h"
#include "ExampleGame/Components/ShadyCamera/ShadyCamera.h"
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
	this->camSpeed = 3.0f;
	this->camMode = CameraMode::CameraMode_Game;
	
	this->newPinch = false;
	this->velocityThreshold = 7.0f;
	this->heightThreshold = 10.0f;

	this->gameObjectRef = (GameObject*)this->GetObject();
	Transform* camTransform = this->gameObjectRef->GetTransform();

	// TODO [Implement] : set this based on device resolution
	this->gameCamHeight = 24.0f;

	// TODO [Implement] : get overview pos and start room from level data
	this->overviewPos = glm::vec3(0.0f, 50.0f, 0.0f);
	this->gameCamPos = glm::vec3(0.0f, this->gameCamHeight, 0.0f);

	camTransform->SetPosition(this->gameCamPos);
	camTransform->Rotate(90.0f inRadians, XAXIS);
	camTransform->Rotate(180.0f inRadians, camTransform->GetForward());

	this->gridManagerRef = nullptr;

	this->addSubscriptionToMessageType(MESSAGE_TYPE_PINCH_GESTURE, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_GRID_ROOM_ENTERED, this->GetTypeId(), false);
}

void ShadyCamera::destroy() {
	this->removeSubscriptionToMessageType(MESSAGE_TYPE_PINCH_GESTURE, this->GetTypeId());
	this->gameObjectRef = nullptr;
	this->gridManagerRef = nullptr;
}

void ShadyCamera::HandleMessage(MessageChunk messageChunk) {
	Camera::HandleMessage(messageChunk);
	switch (messageChunk->GetMessageType()) {
		case MESSAGE_TYPE_PINCH_GESTURE:
			this->onPinch();
			break;

		case MESSAGE_TYPE_GRID_ROOM_ENTERED:
			this->onGridRoomEntered(messageChunk->messageData.i, SINGLETONS->GetGridManager()->GetRoom(messageChunk->messageData.fv1));
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
	ENGINE->GetTween()->TweenPosition(this->gameObjectRef->GetId(), curPos, newPos,
			this->camSpeed, TWEEN_TRANSLATION_CURVE_TYPE_LINEAR, false, ShadyCameraTween::tweenCallback);
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
 
void ShadyCamera::MoveToCurrentRoom() {
	this->MoveTo(this->gameCamPos);
}

void ShadyCamera::MoveToRoom(float x, float z) {
	this->setGameCameraPosition(x, z);
	this->MoveToCurrentRoom();
}

void ShadyCamera::MoveToRoom(GridCell* cell) {
	this->MoveToRoom(cell->x, cell->z);
}

void ShadyCamera::MoveToOverview() {
	this->MoveTo(this->overviewPos);
}

// TODO [Implement] : if needed
void ShadyCamera::LevelStartPan() {

}

void ShadyCamera::setGameCameraPosition(float x, float z) {
	glm::vec3 roomCenter = this->gridManagerRef->GetRoomCenter(x, z);
	// If the above function returns ZERO_VEC3, that means that the position is not within any room.
	// This is not problematic on its own, but we should do nothing in that case.
	if (roomCenter != ZERO_VEC3) {
		this->gameCamPos = roomCenter;
		this->gameCamPos.y += this->gameCamHeight;
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
			this->MoveToCurrentRoom();
		}
		else {
			this->MoveToOverview();
		}
	}
}

void ShadyCamera::onGridRoomEntered(ObjectIdType id, GridRoom* room) {
	ObjectIdType selectedId = this->gridManagerRef->GetSelectedUnitId();
	if ((id == selectedId) && (room != nullptr) && (this->camMode == CameraMode_Game)) {
		glm::vec3 center = room->GetCenter();
		this->MoveToRoom(center.x, center.z);
	}
}

bool ShadyCamera::tryModeSwitch(float velocity) {
	float startY = this->camMode == CameraMode::CameraMode_Game ? this->gameCamHeight : this->overviewPos.y;
	float camY = this->gameObjectRef->GetTransform()->GetPosition().y;
	if(std::abs(velocity) >= this->velocityThreshold || std::abs(startY - camY) >= this->heightThreshold)
	{
		// Zoom in pinch
		if(velocity > 0) {
			this->setCameraMode(CameraMode::CameraMode_Game);
			this->MoveToCurrentRoom();
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

		MessageChunk cameraChange = ENGINE->GetMessageHub()->GetOneFreeMessage();
		cameraChange->SetMessageType(MESSAGE_TYPE_CAMERA_MODE_CHANGED);
		ENGINE->GetMessageHub()->SendMulticastMessage(cameraChange, this->gameObjectRef->GetId());
	}
}

